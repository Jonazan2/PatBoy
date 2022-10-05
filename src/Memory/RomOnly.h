#pragma once
#include "../Memory.h"

/**************************************************************************//**
*
* \brief Specific memory type that implements a regular cartridge without memory banking
*
******************************************************************************/

class RomOnly : public MemoryChip {
	public:
		RomOnly(Memory*, Cartridge *);
		virtual byte read(word address) override;
		virtual void write(word address, byte data) override;
};
