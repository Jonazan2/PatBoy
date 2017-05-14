#include "MBC1.h"

MBC1::MBC1(Memory* memory, Cartridge *cartridge) : MemoryChip{ memory, cartridge }  {
	ramBanks = new byte[0x8000];
    mode = 0;
    currentRAMBank = 0;
    currentROMBank = 1;
    higherRomBankBits = 0;
    ramEnabled = false;
    
    for ( int i = 0; i < 0x8000; i++ ) {
        ramBanks[i] = 0xFF;
    }
    
    currentROMAddress = 0x4000;
    currentRAMAddress = 0;
}

byte MBC1::read(word address) {
    switch ( address & 0xE000 ) {
        case 0x4000:
        case 0x6000: {
            byte* rom = cartridge->getRom();
            return rom[(address - 0x4000) + currentROMAddress];
            break;
        } case 0xA000: {
            if ( ramEnabled ) {
                if ( mode == 0 ) {
                    return ramBanks[address - 0xA000];
                }
                else {
                    return ramBanks[(address - 0xA000) + currentRAMAddress];
                }
            } else {
                return 0xFF;
            }
        
        } default: {
            return memory->readDirectly(address);
        }
    }
}

void MBC1::write(const word address, const byte data) {
    switch ( address & 0xE000 ) {
        case 0x0000: {
            if ( cartridge->getRamSize() != Cartridge::RamSize::NONE_RAM ) {
                ramEnabled = ((data & 0x0F) == 0x0A);
            }
            break;
            
        } case 0x2000: {
            if ( mode == 0 ) {
                currentROMBank = (data & 0x1F) | (higherRomBankBits << 5);
            } else {
                currentROMBank = data & 0x1F;
            }
            
            if (currentROMBank == 0x00 || currentROMBank == 0x20
                || currentROMBank == 0x40 || currentROMBank == 0x60) {
                currentROMBank++;
            }
            
            currentROMBank &= (cartridge->getROMBanks() - 1);
            currentROMAddress = currentROMBank * 0x4000;
            break;
            
        } case 0x4000: {
            if ( mode == 1 ) {
                currentRAMBank = data & 0x03;
                currentRAMBank &= (cartridge->getRamBanks() - 1);
                currentRAMAddress = currentRAMBank * 0x2000;
            } else {
                higherRomBankBits = data & 0x03;
                currentROMBank = (currentROMBank & 0x1F) | (higherRomBankBits << 5);
                
                if (currentROMBank == 0x00 || currentROMBank == 0x20
                    || currentROMBank == 0x40 || currentROMBank == 0x60) {
                    currentROMBank++;
                }
                
                currentROMBank &= (cartridge->getROMBanks() - 1);
                currentROMAddress = currentROMBank * 0x4000;
            }
            break;
            
        } case 0x6000: {
			mode = data & 0x01;
            break;
            
        } case 0xA000: {
            if ( ramEnabled ) {
                if ( mode == 0 ) {
                    ramBanks[address - 0xA000] = data;
                } else {
                    ramBanks[(address - 0xA000) + currentRAMAddress] = data;
                }
            }
            break;
            
        } default: {
            memory->writeDirectly(address, data);
            break;
        }
    }

}