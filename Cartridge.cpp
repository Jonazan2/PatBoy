#include "Cartridge.h"

using namespace std;

Cartridge::Cartridge(const string &filePath) {
    path = filePath;
    loadRom();
    extractHeaderData();
}

void Cartridge::loadRom() {
	FILE *file = fopen(path.c_str(), "rb");
	if (file == nullptr) {
        exit(-1);
	}
    
    fseek(file, 0L, SEEK_END);
    this->romSize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    
	this->rom = new byte[romSize];
	fread(rom, romSize, 1, file);

	fclose(file);
}

long Cartridge::getFileSize(FILE *file) {
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
    
    this->cgbFlag = GameColorFlag(rom[0x0143]);
    this->sgbFlag = SuperGameBoyFlag(rom[0x0146]);
    this->cartridgeType = CartridgeType(rom[0x0147]);
    
    this->cartridgeSize = CartridgeSize(rom[0x0148]);
    this->ramSize = RamSize(rom[0x0149]);
    this->destinationCode = DestinationCode(rom[0x014A]);
    
    extractRamBanks();
    extractRomBanks();
}

void Cartridge::extractRamBanks() {
    int banks;
    string name;
    switch ( ramSize ) {
		case RamSize::NONE_RAM:  banks = 0; break;
        case RamSize::KB_RAM_2:  banks = 0; break;
        case RamSize::KB_RAM_8:  banks = 0; break;
        case RamSize::KB_RAM_32: banks = 4; break;
    }
    this->ramBanks = banks;
}

void Cartridge::extractRomBanks() {
    int banks;
    switch ( cartridgeSize ) {
		case CartridgeSize::KB_32:  banks = 0;  break;
        case CartridgeSize::KB_64:  banks = 4;  break;
        case CartridgeSize::KB_128: banks = 8;  break;
        case CartridgeSize::KB_256: banks = 16; break;
        case CartridgeSize::KB_512: banks = 32; break;
        case CartridgeSize::MB_1:
            if ( cartridgeType == CartridgeType::MBC1 ||
				 cartridgeType == CartridgeType::MBC1_RAM ||
				 cartridgeType == CartridgeType::MBC1_RAM_BATTERY ) {
                banks = 63;
            } else {
                banks = 64;
            }
            break;
        case CartridgeSize::MB_2:
            if ( cartridgeType == CartridgeType::MBC1 ||
				 cartridgeType == CartridgeType::MBC1_RAM ||
				 cartridgeType == CartridgeType::MBC1_RAM_BATTERY ) {
                banks = 125;
            } else {
                banks = 128;
            }
            break;
        case CartridgeSize::MB_4:   banks = 256; break;
        case CartridgeSize::MB_1_1: banks = 72;  break;
        case CartridgeSize::MB_1_2: banks = 80;  break;
        case CartridgeSize::MB_1_5: banks = 96;  break;
    }
    this->romBanks = banks;
}

string Cartridge::getRamSizeName() const {
    string name;
    switch ( ramSize ) {
		case RamSize::NONE_RAM:  name = "No RAM"; break;
        case RamSize::KB_RAM_2:  name = "2 KB";           break;
        case RamSize::KB_RAM_8:  name = "8 KB";           break;
        case RamSize::KB_RAM_32: name = "32 KB";          break;
    }
    return name;
}

string Cartridge::getCartridgeTypeName() const {
    string type;
    switch ( cartridgeType ) {
		case CartridgeType::ROM_ONLY:                type = "ROM only";                          break;
        case CartridgeType::MBC1:                    type = "MBC1";                              break;
        case CartridgeType::MBC1_RAM:                type = "MBC1 with RAM";                     break;
        case CartridgeType::MBC1_RAM_BATTERY:        type = "MBC1 with RAM and battery";         break;
        case CartridgeType::MBC2:                    type = "MBC2";                              break;
        case CartridgeType::MBC2_BATTERY:            type = "MBC2 with battery";                 break;
        case CartridgeType::ROM_RAM:                 type = "ROM with RAM";                      break;
        case CartridgeType::ROM_RAM_BATTERY:         type = "ROM with RAM and battery";          break;
        case CartridgeType::MMM01:                   type = "MMM01";                             break;
        case CartridgeType::MMM01_RAM:               type = "MMM01 with RAM";                    break;
        case CartridgeType::MMM01_RAM_BATTERY:       type = "MMM01 with RAM and battery";        break;
        case CartridgeType::MBC3_TIMER_BATTERY:      type = "MBC3 with timer and battery";       break;
        case CartridgeType::MBC3_TIMER_RAM_BATTERY:  type = "MBC3 with timer, RAM and battery";  break;
        case CartridgeType::MBC3:                    type = "MBC3";                              break;
        case CartridgeType::MBC3_RAM:                type = "MBC3 with RAM";                     break;
        case CartridgeType::MBC3_RAM_BATTERY:        type = "MBC3 with RAM and battery";         break;
        case CartridgeType::MBC4:                    type = "MBC4";                              break;
        case CartridgeType::MBC4_RAM:                type = "MBC4 with RAM";                     break;
        case CartridgeType::MBC4_RAM_BATTERY:        type = "MBC4 with RAM and battery";         break;
        case CartridgeType::MBC5:                    type = "MBC5";                              break;
        case CartridgeType::MBC5_RAM:                type = "MBC5 with RAM";                     break;
        case CartridgeType::MBC5_RAM_BATTERY:        type = "MBC5 with RAM and battery";         break;
        case CartridgeType::MBC5_RUMBLE:             type = "MBC5 with rumble";                  break;
        case CartridgeType::MBC5_RUMBLE_RAM_BATTERY: type = "MBC5 with rumble, RAM and battery"; break;
        case CartridgeType::POCKET_CAMERA:           type = "Pocket camera";                     break;
        case CartridgeType::BANDAI_TAMA5:            type = "Bandai Tama 5";                     break;
        case CartridgeType::HuC3:                    type = "HuC3";                              break;
        case CartridgeType::HuC1_RAM_BATTERY:        type = "HuC1 with RAM and battery";         break;
    }
    return type;
}

string Cartridge::getCartridgeSizeName() const {
    string name;
    switch ( cartridgeSize ) {
		case CartridgeSize::KB_32:	name = "32 KB";  break;
        case CartridgeSize::KB_64:  name = "64 KB";  break;
        case CartridgeSize::KB_128: name = "128 KB"; break;
        case CartridgeSize::KB_256: name = "256 KB"; break;
        case CartridgeSize::KB_512: name = "512 KB"; break;
        case CartridgeSize::MB_1:   name = "1 KB";   break;
        case CartridgeSize::MB_2:   name = "2 KB";   break;
        case CartridgeSize::MB_4:   name = "4 KB";   break;
        case CartridgeSize::MB_1_1: name = "1.1 KB"; break;
        case CartridgeSize::MB_1_2: name = "1.2 KB"; break;
        case CartridgeSize::MB_1_5: name = "1.5 KB"; break;
    }
    return name;
}

string Cartridge::getCGBFlagName() const {
    string cgb;
    if ( cgbFlag == GameColorFlag::CGB_SUPPORT ) {
        cgb = "CGB functions";
    } else if ( cgbFlag == GameColorFlag::CGB_ONLY ) {
        cgb = "CGB game";
    } else {
        cgb = "-";
    }
    return cgb;
}

string Cartridge::getSGBFlagName() const {
    string sgb;
    if ( sgbFlag == SuperGameBoyFlag::SGB_FUNCTIONS ) {
        sgb = "supported";
    } else if ( sgbFlag == SuperGameBoyFlag::NO_SGB ) {
        sgb = "not supported";
    }
    return sgb;
}

string Cartridge::getDestinationCodeName() const {
    string code;
    if ( destinationCode == DestinationCode::NO_JAPAN ) {
        code = "No Japan";
    } else if ( destinationCode == DestinationCode::JAPAN ) {
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

Cartridge::~Cartridge(){
    delete rom;
}