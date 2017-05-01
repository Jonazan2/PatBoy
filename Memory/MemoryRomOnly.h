#ifndef _MEMORY_ROM_ONLY_H_
#define _MEMORY_ROM_ONLY_H_

#include "Memory.h"

/**************************************************************************//**
*
* \brief Specific memory type that implements a regular cartridge without memory banking
*
******************************************************************************/

class MemoryRomOnly : public Memory {
    
private:
    byte readWithChip(const word) const;
    void writeWithChip(const word, const byte);
    
public:
    MemoryRomOnly(Cartridge *, Audio *, Joypad *joypad);
};

#endif
