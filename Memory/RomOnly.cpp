#include "RomOnly.h"

RomOnly::RomOnly(Memory* memory, Cartridge* cartridge) : MemoryChip(memory, cartridge) {}

byte RomOnly::read(word address) {
	if ( address >= 0xA000 && address < 0xC000 ) {
        if ( cartridge->getRamSize() != Cartridge::RamSize::NONE_RAM ) {
            return memory->readDirectly(address);
        } else {
            return 0xFF;
        }
    } else {
        return memory->readDirectly(address);
    }
}

void RomOnly::write(word address, byte data) {
    if ( address < 0x8000 ) {
        // trapped address
    } else if ( address >= 0xA000 && address < 0xC000 ) {
        if ( cartridge->getRamSize() != Cartridge::RamSize::NONE_RAM ) {
            memory->writeDirectly(address, data);
        }
    } else {
		memory->writeDirectly(address, data);
    }
}