#include "MemoryFactory.h"


Memory * MemoryFactory::createMemoryChip(Cartridge * cartridge, Audio * audio, Joypad *joypad) {
    Memory * memory = nullptr;
    switch ( cartridge->getCartridgeType() ) {
		case Cartridge::CartridgeType::MBC1:                    memory = new  MemoryMBC1(cartridge, audio, joypad);       break;
        case Cartridge::CartridgeType::MBC1_RAM:                memory = new  MemoryMBC1(cartridge, audio, joypad);       break;
        case Cartridge::CartridgeType::MBC1_RAM_BATTERY:        memory = new  MemoryMBC1(cartridge, audio, joypad);       break;
        case Cartridge::CartridgeType::MBC2:                    memory = new  MemoryMBC2(cartridge, audio,joypad);        break;
        case Cartridge::CartridgeType::MBC2_BATTERY:            memory = new  MemoryMBC2(cartridge, audio, joypad);       break;
        case Cartridge::CartridgeType::ROM_RAM:                 memory = new  MemoryRomOnly(cartridge, audio, joypad);    break;
        case Cartridge::CartridgeType::ROM_RAM_BATTERY:         memory = new  MemoryRomOnly(cartridge, audio, joypad);    break;
        case Cartridge::CartridgeType::MBC3_TIMER_RAM_BATTERY:  memory = new  MemoryMBC3(cartridge, audio, joypad);       break;
        case Cartridge::CartridgeType::MBC3:                    memory = new  MemoryMBC3(cartridge, audio, joypad);       break;
        case Cartridge::CartridgeType::MBC3_RAM:                memory = new  MemoryMBC3(cartridge, audio, joypad);       break;
        case Cartridge::CartridgeType::MBC3_RAM_BATTERY:        memory = new  MemoryMBC3(cartridge, audio, joypad);       break;
        case Cartridge::CartridgeType::ROM_ONLY:
        default:												memory = new  MemoryRomOnly(cartridge, audio, joypad);    break;
    }
    return memory;
}