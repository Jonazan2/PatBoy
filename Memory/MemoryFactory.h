#ifndef _MEMORY_FACTORY_H_
#define _MEMORY_FACTORY_H_

#include <iostream>
#include "Memory.h"
#include "Cartridge.h"
#include "MemoryRomOnly.h"
#include "MemoryMBC1.h"
#include "MemoryMBC2.h"
#include "MemoryMBC3.h"

/**************************************************************************//**
 *
 * \brief Factory that creates a memory instance from the cartridge type.
 *
******************************************************************************/

class MemoryFactory {
  
public:
    Memory* createMemoryChip(Cartridge *, Audio *, Joypad *);
};

#endif 
