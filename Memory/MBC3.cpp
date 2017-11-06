#include "MBC3.h"

MBC3::MBC3(Memory* memory, Cartridge *cartridge) : MemoryChip{ memory, cartridge }  {
	ram = new byte[RAM_BANK_SIZE * 4];
	memset(ram, 0xFF, sizeof(ram));
    ramEnabled = false;

    currentRAMBank = 0;
    currentROMBank = 1;
}

byte MBC3::read(word address) {
	if (address <= 0x3FFF) {
		return memory->readDirectly(address);
	} else if (address <= 0x7FFF) {
		return readFromRomBank(address);
	} else if (address <= 0x9FFF) {
		return memory->readDirectly(address);
	} else if (address <= 0xBFFF) {
		return readFromRamBank(address);
	}
}

byte MBC3::readFromRomBank(word address) {
	int romAddress = currentROMBank * ROM_BANK_SIZE;
	byte* rom = cartridge->getRom();
	return rom[(address - 0x4000) + romAddress];
}

byte MBC3::readFromRamBank(word address) {
	if (ramEnabled) {
		int ramAddress = 0;
		if (currentRAMBank != 0) {
			ramAddress = currentRAMBank * RAM_BANK_SIZE;
		}
		return ram[(address - 0xA000) + ramAddress];
	} else {
		return 0xFF;
	}
}

void MBC3::write(const word address, const byte data) {
	if (address <= 0x1FFF) {
		if (cartridge->getRamSize() != Cartridge::RamSize::NONE_RAM) {
			ramEnabled = ((data & 0x0F) == 0x0A);
		}
	} else if (address >= 0x2000 && address <= 0x3FFF) {
		setRomBank(data);
	} else if (address >= 0x4000 && address <= 0x5FFF) {
		setRamBank(data);
	} else if (address >= 0x6000 && address <= 0x7FFF) {
		// TODO: implement RTC clock
	} else if (address >= 0xA000 && address <= 0xBFFF) {
		writeToRamBank(address, data);
	} else {
		memory->writeDirectly(address, data);
	}
}

void MBC3::setRomBank(byte data) {
	currentROMBank = (data & 0x7F);

	for (byte specialBank : SPECIAL_ROM_BANKS) {
		if (currentROMBank == specialBank) {
			currentROMBank++;
		}
	}

	currentROMBank &= (cartridge->getROMBanks() - 1);
}

void MBC3::setRamBank(byte data) {
	if (data <= 0x03) {
		currentRAMBank = data;
	}
}

void MBC3::writeToRamBank(word address, byte data) {
	if (ramEnabled) {
		word ramAddress = currentRAMBank * RAM_BANK_SIZE;
		ram[(address - 0xA000) + ramAddress] = data;
	}
}