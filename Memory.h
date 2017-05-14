#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "Types.h"
#include "Cartridge.h"
#include "Audio.h"
#include "Joypad.h"
#include "CPU.h"
#include "Memory/MemoryChip.h"

class CPU;
class Joypad;

/**************************************************************************//**
 * \brief Class that implements the Memory system of the Nintendo Game Boy
 *
 * This class represent the memory of the Game Boy. It has an array of bytes
 * to represent the Memory and methods to read and write into memory.
 *
 * See also \ref CPU \ref Memmory \ref Utils
******************************************************************************/
class Memory {
public:
    Memory(Cartridge *, Audio *, Joypad *);
	virtual ~Memory();

	void init(CPU *);
	void reset();
    
    byte read(const word) const;
	inline byte Memory::readDirectly(const word address) const {
		return map[address];
	}

	inline word Memory::readWordDirectly(const word address) const {
		Register aux;
		aux.low = map[address];
		aux.hi = map[address + 1];
		return aux.value;
	}

    void write(const word, const byte);

	inline void Memory::writeDirectly(const word address, const byte data) {
		this->map[address] = data;
	}

	inline byte * getMap() const {
		return map;
	}

private:

	static MemoryChip* createMemoryChipForCartridge(Memory* memory, Cartridge* cartridge);

    Audio *audio;
    Joypad *joypad;
    CPU *cpu;
    byte *map;
    Cartridge *cartridge;
	MemoryChip* chip;


    void loadCartridge() const;
    void DMA(byte);
};

#endif
