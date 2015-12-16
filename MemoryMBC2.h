#ifndef _MEMORY_MBC2_H_
#define _MEMORY_MBC2_H_

#include "Memory.h"

/**************************************************************************//**
 *
 * \brief Specific memory type that implements the MBC2 chip
 *
******************************************************************************/

class MemoryMBC2 : public Memory {

private:
    int currentRomBank;
    int currentRomAddress;
    bool ramEnabled;
    
public:
    MemoryMBC2(Cartridge *, Audio *, Joypad *);
    byte readWithChip(const word) const;
    void writeWithChip(const word, const byte);
    
};


#endif