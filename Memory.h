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
    Utils *utils;
    
    void loadCartridge() const;
    void DMA(byte);
    byte readIORegister(const word) const;
    byte readCommon(const word) const;
    void writeCommon(const word, const byte);
    void writeIORegister(const word, const byte);
    
public:
    Memory(Cartridge *, Audio *, Joypad *);
    void init(CPU *);
    
    byte read(const word) const;
    virtual byte readWithChip(const word) const = 0;
    byte readDirectly(const word) const;
    
    void write(const word, const byte);
    virtual void writeWithChip(const word, const byte) = 0;
    void writeDirectly(const word, const byte);
    
    void dumpHexadecimalMemory() const;
    void reset();
    virtual ~Memory();
};

#endif
