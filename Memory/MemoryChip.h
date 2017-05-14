#pragma once
#include "Memory.h"

class Memory;

class MemoryChip {

public:
	virtual byte read(word address) = 0;
	virtual void write(word address, byte data) = 0; 

protected:
	MemoryChip(Memory* memory, Cartridge* cartridge) {
		this->memory = memory;
		this->cartridge = cartridge;
	}

	Memory* memory;
	Cartridge* cartridge;
};