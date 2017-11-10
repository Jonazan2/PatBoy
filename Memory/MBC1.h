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
		~MBC1();

		byte read(word address) final override;
		void write(word address, byte data) final override;
		void save(std::string &name) override;
		void load(std::string &name) override;

	private:
		const byte SPECIAL_ROM_BANKS[4] = {0x00, 0x20, 0x40, 0x60};
		const word RAM_BANK_SIZE		= 0x2000;
		const word ROM_BANK_SIZE		= 0x4000;

		enum class Mode : int {
			ROM = 0,
			RAM = 1
		};

		Mode mode;
		byte currentRAMBank;
		byte currentROMBank;
		byte higherRomBankBits;

		byte* ram;
		bool ramEnabled;

		byte readFromRamBank(word address);
		byte readFromRomBank(word address);

		void writeToRamBank(word address, byte data);
		void setRomBank(byte data);
		void setRamOrUpperBitsOfRomBank(byte data);
		void setMode(byte data);
};
