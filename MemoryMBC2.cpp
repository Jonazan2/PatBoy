#include "MemoryMBC2.h"

MemoryMBC2::MemoryMBC2(Cartridge *cartridge, Audio *audio, Joypad *joypad):Memory(cartridge, audio, joypad)  {
    currentRomBank = 1;
    currentRomAddress = 0x4000;
    ramEnabled = false;
}

byte MemoryMBC2::readWithChip(const word address) const {
    switch ( address & 0xE000 ) {
        case 0x4000:
        case 0x6000: {
            byte* rom = cartridge->getRom();
            return rom[(address - 0x4000) + currentRomAddress];
            
        } case 0xA000: {
            if ( address < 0xA200 ) {
                if ( ramEnabled ) {
                    return this->readDirectly(address);
                } else {
                    return 0xFF;
                }
            } else {
                return 0x00;
            }
            
        } default: {
            return this->readDirectly(address);
        }
    }
}

void MemoryMBC2::writeWithChip(const word address, const byte data) {
    switch ( address & 0xE000 ) {
        case 0x0000: {
            if ( !(address & 0x0100) ) {
                ramEnabled = (data & 0x0F) == 0x0A;
            }
            break;
            
        } case 0x2000: {
            if ( address & 0x0100 ) {
                currentRomBank = data & 0x0F;
                
                if ( currentRomBank == 0 ) {
                    currentRomBank = 1;
                }
                
                currentRomBank &= (cartridge->getROMBanks() - 1);
                currentRomAddress = currentRomBank * 0x4000;
            }
            break;
        }
        
        case 0x4000:
        case 0x6000: {
            //trapped area
            break;
        }
            
        case 0xA000: {
            if ( address < 0xA200 ) {
                if (ramEnabled) {
                    this->writeDirectly(address, data & 0x0F);
                }
            }
            break;
            
        } default: {
            this->writeDirectly(address, data);
            break;
        }
    }
}