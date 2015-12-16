#include "MemoryFactory.h"


Memory * MemoryFactory::createMemoryChip(Cartridge * cartridge, Audio * audio, Joypad *joypad) {
    Memory * memory = NULL;
    
    switch ( cartridge->getCartridgeType() ) {
        case Cartridge::MBC1:                    memory = new  MemoryMBC1(cartridge, audio, joypad);       break;
        case Cartridge::MBC1_RAM:                memory = new  MemoryMBC1(cartridge, audio, joypad);       break;
        case Cartridge::MBC1_RAM_BATTERY:        memory = new  MemoryMBC1(cartridge, audio, joypad);       break;
        case Cartridge::MBC2:                    memory = new  MemoryMBC2(cartridge, audio,joypad);        break;
        case Cartridge::MBC2_BATTERY:            memory = new  MemoryMBC2(cartridge, audio, joypad);       break;
        case Cartridge::ROM_RAM:                 memory = new  MemoryRomOnly(cartridge, audio, joypad);    break;
        case Cartridge::ROM_RAM_BATTERY:         memory = new  MemoryRomOnly(cartridge, audio, joypad);    break;
        case Cartridge::MBC3_TIMER_RAM_BATTERY:  memory = new  MemoryMBC3(cartridge, audio, joypad);       break;
        case Cartridge::MBC3:                    memory = new  MemoryMBC3(cartridge, audio, joypad);       break;
        case Cartridge::MBC3_RAM:                memory = new  MemoryMBC3(cartridge, audio, joypad);       break;
        case Cartridge::MBC3_RAM_BATTERY:        memory = new  MemoryMBC3(cartridge, audio, joypad);       break;
        case Cartridge::ROM_ONLY:
        default:                                 memory = new  MemoryRomOnly(cartridge, audio, joypad);    break;
    }
    
    return memory;
}