#pragma once
#include "Memory.h"

/**************************************************************************//**
 *
 * \brief Specific memory type that implements the MBC1 chip
 *
******************************************************************************/

class MBC1 : public MemoryChip {
	public:
		MBC1(Memory* memory, Cartridge* cartridge);
		virtual byte read(word address) override ;
		void write(word address, byte data) override;
    
	private:
		int mode;
		int currentRAMBank;
		int currentROMBank;
		bool ramEnabled;
		byte higherRomBankBits;
		byte* ramBanks;
		int currentROMAddress;
		int currentRAMAddress;
};
