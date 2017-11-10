#pragma once
#include "Memory.h"

#include <string>

class Memory;

class MemoryChip {

public:
	virtual byte read(word address) = 0;
	virtual void write(word address, byte data) = 0;
	virtual void save(std::string &name) {};
	virtual void load(std::string &name) {};

protected:

	const std::string SAVE_FILE_EXTENSION = ".sav";

	MemoryChip(Memory* memory, Cartridge* cartridge) {
		this->memory = memory;
		this->cartridge = cartridge;
	}

	Memory* memory;
	Cartridge* cartridge;
};