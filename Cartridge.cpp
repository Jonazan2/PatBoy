#include "Cartridge.h"

using namespace std;

Cartridge::Cartridge(string filePath) {
    this->path = filePath;
    this->file = NULL;
    this->hasRealTimeClock = false;
    this->currentRTCValue = 0;
    loadRom();
    extractHeaderData();
}

void Cartridge::loadRom() {
	if ( (file = fopen(path.c_str(), "rb")) == NULL ) {
        exit(-1);
	}
    
    fseek(this->file, 0L, SEEK_END);
    this->romSize = ftell(this->file);
    fseek(file, 0L, SEEK_SET);
    
	this->rom = new byte[romSize];
	fread(rom, romSize, 1, file);
}

long Cartridge::getFileSize() {
	long currentPosition, endPosition;
	currentPosition = ftell(file);
	fseek(file, 0, 2);
	endPosition = ftell(file);
	fseek(file, currentPosition, 0);
	return endPosition;
}

void Cartridge::extractHeaderData() {
    char *nameByte = new char[16];
    
	for ( int i = 0; i < 15; i++ ) {
        nameByte[i] = rom[i+0x0134];
    }
    title = nameByte;
    
    this->cgbFlag = CGB_Flag(rom[0x0143]);
    this->sgbFlag = SGB_Flag(rom[0x0146]);
    this->cartridgeType = CartridgeType(rom[0x0147]);
    
    if ( cartridgeType ==     MBC3_TIMER_BATTERY || cartridgeType == MBC3_TIMER_RAM_BATTERY ) {
        hasRealTimeClock = true;
    }
    
    this->cartridgeSize = CartridgeSize(rom[0x0148]);
    this->ramSize = RamSize(rom[0x0149]);
    this->destinationCode = DestinationCode(rom[0x014A]);
    
    extractRamBanks();
    extractRomBanks();
}

void Cartridge::printHeaderData() const {
    
    cout << "===== " << title << " =====" << endl;
    cout << "CGB: " << getCGBFlagName() << endl;
    cout << "SGB: " << getSGBFlagName() << endl;
    cout << "Cartridge type: " << getCartridgeTypeName() << endl;
    cout << "Rom size: " << getCartridgeSizeName() <<endl ;
    cout << "Number of rom banks: " << romBanks << endl;
    cout << "Ram size: " << getRamSizeName() << endl;
    cout << "Number of RAM banks: " << ramBanks << endl;
    cout << "Destination code: " << getDestinationCodeName() << endl;
}

void Cartridge::extractRamBanks() {
    int banks;
        string name;
        switch ( ramSize ) {
            case NONE_RAM:  banks = 0; break;
            case KB_RAM_2:  banks = 0; break;
            case KB_RAM_8:  banks = 0; break;
            case KB_RAM_32: banks = 4; break;
        }
    this->ramBanks = banks;
}

void Cartridge::extractRomBanks() {
    int banks;
    switch ( cartridgeSize ) {
        case KB_32:  banks = 0;  break;
        case KB_64:  banks = 4;  break;
        case KB_128: banks = 8;  break;
        case KB_256: banks = 16; break;
        case KB_512: banks = 32; break;
        case MB_1:
            if ( cartridgeType == MBC1 || cartridgeType == MBC1_RAM || cartridgeType == MBC1_RAM_BATTERY ) {
                banks = 63;
            } else {
                banks = 64;
            }
            break;
        case MB_2:
            if ( cartridgeType == MBC1 || cartridgeType == MBC1_RAM || cartridgeType == MBC1_RAM_BATTERY ) {
                banks = 125;
            } else {
                banks = 128;
            }
            break;
        case MB_4:   banks = 256; break;
        case MB_1_1: banks = 72;  break;
        case MB_1_2: banks = 80;  break;
        case MB_1_5: banks = 96;  break;
    }
    this->romBanks = banks;
}

string Cartridge::getRamSizeName() const {
    string name;
    switch ( ramSize ) {
        case NONE_RAM:  name = "Cartridge without RAM"; break;
        case KB_RAM_2:  name = "2 Kilobytes";           break;
        case KB_RAM_8:  name = "8 Kylobytes";           break;
        case KB_RAM_32: name = "32 Kylobytes";          break;
    }
    return name;
}

string Cartridge::getCartridgeTypeName() const {
    string type;
    switch ( cartridgeType ) {
        case ROM_ONLY:                type = "ROM only";                          break;
        case MBC1:                    type = "MBC1";                              break;
        case MBC1_RAM:                type = "MBC1 with RAM";                     break;
        case MBC1_RAM_BATTERY:        type = "MBC1 with RAM and battery";         break;
        case MBC2:                    type = "MBC2";                              break;
        case MBC2_BATTERY:            type = "MBC2 with battery";                 break;
        case ROM_RAM:                 type = "ROM with RAM";                      break;
        case ROM_RAM_BATTERY:         type = "ROM with RAM and battery";          break;
        case MMM01:                   type = "MMM01";                             break;
        case MMM01_RAM:               type = "MMM01 with RAM";                    break;
        case MMM01_RAM_BATTERY:       type = "MMM01 with RAM and battery";        break;
        case MBC3_TIMER_BATTERY:      type = "MBC3 with timer and battery";       break;
        case MBC3_TIMER_RAM_BATTERY:  type = "MBC3 with timer, RAM and battery";  break;
        case MBC3:                    type = "MBC3";                              break;
        case MBC3_RAM:                type = "MBC3 with RAM";                     break;
        case MBC3_RAM_BATTERY:        type = "MBC3 with RAM and battery";         break;
        case MBC4:                    type = "MBC4";                              break;
        case MBC4_RAM:                type = "MBC4 with RAM";                     break;
        case MBC4_RAM_BATTERY:        type = "MBC4 with RAM and battery";         break;
        case MBC5:                    type = "MBC5";                              break;
        case MBC5_RAM:                type = "MBC5 with RAM";                     break;
        case MBC5_RAM_BATTERY:        type = "MBC5 with RAM and battery";         break;
        case MBC5_RUMBLE:             type = "MBC5 with rumble";                  break;
        case MBC5_RUMBLE_RAM_BATTERY: type = "MBC5 with rumble, RAM and battery"; break;
        case POCKET_CAMERA:           type = "Pocket camera";                     break;
        case BANDAI_TAMA5:            type = "Bandai Tama 5";                     break;
        case HuC3:                    type = "HuC3";                              break;
        case HuC1_RAM_BATTERY:        type = "HuC1 with RAM and battery";         break;
    }
    return type;
}

string Cartridge::getCartridgeSizeName() const {
    string name;
    switch ( cartridgeSize ) {
        case KB_32:  name = "32 Kilobytes";  break;
        case KB_64:  name = "64 Kilobytes";  break;
        case KB_128: name = "128 Kilobytes"; break;
        case KB_256: name = "256 Kilobytes"; break;
        case KB_512: name = "512 Kilobytes"; break;
        case MB_1:   name = "1 Megabytes";   break;
        case MB_2:   name = "2 Megabytes";   break;
        case MB_4:   name = "4 Megabytes";   break;
        case MB_1_1: name = "1.1 Megabytes"; break;
        case MB_1_2: name = "1.2 Megabytes"; break;
        case MB_1_5: name = "1.5 Megabytes"; break;
    }
    return name;
}

string Cartridge::getCGBFlagName() const {
    string cgb;
    if ( cgbFlag == CGB_SUPPORT ) {
        cgb = "Game Boy Color functions supported";
    } else if ( cgbFlag == CGB_ONLY ) {
        cgb = "Game Boy Color game";
    } else {
        cgb = "No Game Boy Color";
    }
    return cgb;
}

string Cartridge::getSGBFlagName() const {
    string sgb;
    if ( sgbFlag == SGB_FUNCTIONS ) {
        sgb = "Super Game Boy functions supported";
    } else if ( sgbFlag == NO_SGB ) {
        sgb = "Super Game Boy functions not supported";
    }
    return sgb;
}

string Cartridge::getDestinationCodeName() const {
    string code;
    if ( destinationCode == NO_JAPAN ) {
        code = "No Japan";
    } else if ( destinationCode == JAPAN ) {
        code = "Japan";
    }
    return code;
}

void Cartridge::dumpHexadecimalFile() const {
    cout << "\n\t\t\t\t\t==== Hex dump of " <<  title << " ==== " << endl;
    long i;
    unsigned char buff[17];
    
    for ( i = 0; i < romSize; i++ ) {
        
        if ( (i % 16) == 0 ) {
            if ( i != 0 ){
                printf (" %s\n", buff);
            }
            printf ("%08lX ", i);
        }
        
        printf (" %02x", rom[i]);
        
        if ( (rom[i] < 0x20) || (rom[i] > 0x7e) ) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = rom[i];
        }
        buff[(i % 16) + 1] = '\0';
    }
    printf (" %s\n", buff);
}

string Cartridge::getTitle() const {
    return this->title;
}

Cartridge::DestinationCode Cartridge::getDestinationCode() const {
    return this->destinationCode;
}

Cartridge::CartridgeSize Cartridge::getCartridgeSize() const {
    return this->cartridgeSize;
}

Cartridge::CartridgeType Cartridge::getCartridgeType() const {
    return this->cartridgeType;
}

Cartridge::RamSize Cartridge::getRamSize() const {
    return this->ramSize;
}

int Cartridge::getROMBanks() const {
    return this->romBanks;
}

int Cartridge::getRamBanks() const {
    return this->ramBanks;
}

long Cartridge::getRomSize() const {
    return this->romSize;
}

byte* Cartridge::getRom() const {
    return this->rom;
}

bool Cartridge::hasRTC() const {
    return hasRealTimeClock;
}

time_t Cartridge::getCurrentRTCValue() const {
    return currentRTCValue;
}

void Cartridge::updateRTC() {
    time(&currentRTCValue);
}

Cartridge::~Cartridge(){
    fclose(file);
    delete rom;
}