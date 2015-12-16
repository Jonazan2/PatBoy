#ifndef _MEMORY_MBC1_H_
#define _MEMORY_MBC1_H_

#include "Memory.h"

/**************************************************************************//**
 *
 * \brief Specific memory type that implements the MBC1 chip
 *
******************************************************************************/

class MemoryMBC1 : public Memory {
    
private:
    int mode;
    int currentRAMBank;
    int currentROMBank;
    bool ramEnabled;
    byte higherRomBankBits;
    byte* ramBanks;
    int currentROMAddress;
    int currentRAMAddress;
    
public:
    MemoryMBC1(Cartridge *, Audio *, Joypad *);
    byte readWithChip(const word) const;
    void writeWithChip(const word, const byte);
    
};


#endif