#include "Memory.h"

Memory::Memory(Cartridge *cartridge, Audio *audio, Joypad *joypad) {
    this->map = new byte[0x10000];
	memset(map, 0, 0x10000);

    this->cartridge = cartridge;
    this->audio = audio;
    this->joypad = joypad;
    loadCartridge();
}

void Memory::init(CPU * cpu) {
    this->cpu = cpu;
}

byte Memory::read(const word address) const {
	if (address >= 0x4000 && address <= 0x7FFF) {
		return readWithChip(address);
	} else if (address == 0xFF00) {
		return joypad->getState();
	} else {
		return readDirectly(address);
	}
}

void Memory::write(const word address, const byte data) {
    if (address < 0x8000) {
        writeWithChip(address, data);
    } else if ( address < 0xA000) {
        map[address] = data;
    } else if ( (address >= 0xC000) && (address <= 0xDFFF) ){
        map[address] = data;
    } else if ( (address > 0xE000) && (address <= 0xFDFF) ) {
        map[address] = data;
        map[address -0x2000] = data;
    } else if ((address >= 0xFEA0) && (address <= 0xFEFF)) {
        // Not allowed
    } else if (address == 0xFF04) {
        map[address] = 0x0;
        cpu->resetDivRegister();
    }else if (address == 0xFF07) {
        map[address] = data ;
            
        int timerVal = data & 0x03 ;
            
        int clockSpeed = 0 ;
        switch(timerVal) {
            case 0:  clockSpeed = 1024;  break;
            case 1:  clockSpeed = 16;    break;
            case 2:  clockSpeed = 64;    break;
            case 3:  clockSpeed = 256;   break;
            default: assert(false);      break;
        }
            
        if (clockSpeed != cpu->getCurrrentClockSpeed()){
            cpu->resetTimaRegister();
            cpu->setCurrentClockSpeed(clockSpeed);
        }
    } else if (address == 0xFF41) {
        writeDirectly(0xFF41, 0x0);
    } else if (address == 0xFF46){
        for (int i = 0; i < 0xA0; i++) {
            map[0xFE00 + i] = readDirectly((cpu->getAF().hi << 8) + i);
        }
    } else if ((address >= 0xFF4C) && (address <= 0xFF7F)) {
        // Not allowed
    } else if ( address >= 0xFF10 && address <= 0xFF3F ) {
        audio->writeAudioRegister(address, data);
    } else {
		writeDirectly(address, data);
    }
}

void Memory::DMA(byte data) {
    for ( int i = 0; i < 0xA0; i++ ) {
        writeDirectly(0xFE00+i, readDirectly((cpu->getAF().hi << 8) + i));
    }
}

void Memory::dumpHexadecimalMemory() const {
	std::cout << "\n\t\t\t\t\t\t==== Memory ==== " << std::endl;
    long i;
    unsigned char buff[17];
    
    for ( i = 0; i < 0x10000; i++ ) {
        
        if ( (i % 16) == 0 ) {
            if ( i != 0 ){
                printf(" %s\n", buff);
            }
            
            if ( i == 0x0000 ) {
                std::cout << "\n\t\t\t\t\t_ Cartridge fixed bank _" << std::endl;
            } else if ( i == 0x4000 ) {
                std::cout << "\n\t\t\t\t\t_ Cartridge ROM bank _" << std::endl;
            } else if ( i == 0x8000 ) {
                std::cout << "\n\t\t\t\t\t_ Video RAM _" << std::endl;
            } else if ( i == 0xA000 ) {
                std::cout << "\n\t\t\t\t\t_ External RAM _" << std::endl;
            } else if ( i == 0xC000 ) {
                std::cout << "\n\t\t\t\t\t_ Work RAM bank 0 _" << std::endl;
            } else if ( i == 0xD000 ) {
                std::cout << "\n\t\t\t\t\t_ Work RAM bank 1 _" << std::endl;
            } else if ( i == 0xE000 ) {
                std::cout << "\n\t\t\t\t\t_ Work RAM bank 0 _" << std::endl;
            } else if ( i == 0xFE00 ) {
                std::cout << "\n\t\t\t\t\t_ Sprite Attribute Table _" << std::endl;
            } else if ( i == 0xFEA0 ) {
                std::cout << "\n\t\t\t\t\t_ Not usable _" << std::endl;
            } else if ( i == 0xFF00 ) {
                std::cout << "\n\t\t\t\t\t_ I/O Ports _" << std::endl;
            } else if ( i == 0xFF80 ) {
                std::cout << "\n\t\t\t\t\t_ High RAM _" << std::endl;
            }
            printf("%04lX ", i) ;
        }
    
        printf(" %02x", map[i]) ;
        
        
        if ( (map[i] < 0x20) || (map[i] > 0x7E) ) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = map[i];
        }
        buff[(i % 16) + 1] = '\0';
    }
    printf("%s\n", buff) ;
}

void Memory::loadCartridge() const {
    byte *rom = cartridge->getRom();
    for ( int i = 0; i < 0x8000; i++ ) {
        map[i] = rom[i];
    }
}

void Memory::reset() {
	map[0xFF05] = 0x00;
	map[0xFF06] = 0x00;
	map[0xFF07] = 0x00;
	map[0xFF10] = 0x80;
	map[0xFF11] = 0xBF;
	map[0xFF12] = 0xF3;
	map[0xFF14] = 0xBF;
	map[0xFF16] = 0x3F;
	map[0xFF17] = 0x00;
	map[0xFF19] = 0xBF;
	map[0xFF1A] = 0x7F;
	map[0xFF1B] = 0xFF;
	map[0xFF1C] = 0x9F;
	map[0xFF1E] = 0xBF;
	map[0xFF20] = 0xFF;
	map[0xFF21] = 0x00;
	map[0xFF22] = 0x00;
	map[0xFF23] = 0xBF;
	map[0xFF24] = 0x77;
	map[0xFF25] = 0xF3;
	map[0xFF26] = 0xF1;
	map[0xFF40] = 0x91;
	map[0xFF42] = 0x00;
	map[0xFF43] = 0x00;
	map[0xFF45] = 0x00;
	map[0xFF47] = 0xFC;
	map[0xFF48] = 0xFF;
	map[0xFF49] = 0xFF;
	map[0xFF4A] = 0x00;
	map[0xFF4B] = 0x00;
	map[0xFFFF] = 0x00;
}

Memory::~Memory() {
    delete []map;
}
