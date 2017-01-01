#include "MemoryRomOnly.h"

MemoryRomOnly::MemoryRomOnly(Cartridge *cartridge, Audio *audio, Joypad *joypad):Memory(cartridge, audio, joypad) {}

byte MemoryRomOnly::readWithChip(const word address) const {
    if ( address >= 0xA000 && address < 0xC000 ) {
        if ( cartridge->getRamSize() != Cartridge::RamSize::NONE_RAM ) {
            return readDirectly(address);
        } else {
            return 0xFF;
        }
    } else {
        return readDirectly(address);
    }
}

void MemoryRomOnly::writeWithChip(const word address, const byte data) {
    if ( address < 0x8000 ) {
        // trapped address
    } else if ( address >= 0xA000 && address < 0xC000 ) {
        if ( cartridge->getRamSize() != Cartridge::RamSize::NONE_RAM ) {
            writeDirectly(address, data);
        }
    } else {
        writeDirectly(address, data);
    }
}