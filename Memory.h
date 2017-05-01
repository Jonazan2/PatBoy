#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "Types.h"
#include "Cartridge.h"
#include "Audio.h"
#include "Joypad.h"
#include "CPU.h"

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
    
protected:
    Audio *audio;
    Joypad *joypad;
    CPU *cpu;
    byte *map;
    Cartridge *cartridge;
    
    void loadCartridge() const;
    void DMA(byte);
    
public:
    Memory(Cartridge *, Audio *, Joypad *);
    void init(CPU *);
    
    byte read(const word) const;
    virtual byte readWithChip(const word) const = 0;

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
    virtual void writeWithChip(const word, const byte) = 0;

	inline void Memory::writeDirectly(const word address, const byte data) {
		this->map[address] = data;
	}

    void dumpHexadecimalMemory() const;
    void reset();
    virtual ~Memory();
};

#endif
