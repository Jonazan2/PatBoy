#pragma once
#include "../Memory.h"

/**************************************************************************//**
 *
 * \brief Specific memory type that implements the MBC2 chip
 *
******************************************************************************/

class MBC2 : public MemoryChip {
	public:
		MBC2(Memory* memory, Cartridge* cartridge);
		~MBC2();

		byte read(word address) final override;
		void write(word address, byte data) final override;
		void save(const std::string &name) override;
		void load(const std::string &name) override;

	private:
		const word RAM_SIZE				= 0x200;
		const word ROM_BANK_SIZE		= 0x4000;

		byte* ram;
		byte currentROMBank;
		bool ramEnabled;

		byte readFromRam(word address);
		byte readFromRomBank(word address);
		void setRomBank(word address, byte data);
		void setRamState(word address);
};
