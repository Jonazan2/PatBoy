#ifndef CARTRIDGE_H_
#define CARTRIDGE_H_

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include "Types.h"

/*************************************************************//**
 * \brief Class that models a game boy cartridge
 *
 * This class represent a Game Boy cartridge. It has
 * all the data from the cartridge headers and the cartridge rom
 * itself. It has methods to charge a game from its file and to
 * extract all its data from the cartridge header.
*****************************************************************/

class Cartridge {

public:  
    /*! Cartridge's types */
	enum class CartridgeType : byte {
		ROM_ONLY	 			= 0x00,
		MBC1		 			= 0x01,
        MBC1_RAM				= 0x02,
        MBC1_RAM_BATTERY 		= 0x03,
        MBC2 					= 0x05,
        MBC2_BATTERY 			= 0x06,
        ROM_RAM 				= 0x08,
        ROM_RAM_BATTERY 		= 0x09,
        MMM01 					= 0x0B,
        MMM01_RAM				= 0x0C,
        MMM01_RAM_BATTERY 		= 0x0D,
        MBC3_TIMER_BATTERY		= 0x0F,
        MBC3_TIMER_RAM_BATTERY	= 0x10,
        MBC3 					= 0x11,
        MBC3_RAM 				= 0x12,
        MBC3_RAM_BATTERY 		= 0x13,
        MBC4 					= 0x15,
        MBC4_RAM 				= 0x16,
        MBC4_RAM_BATTERY 		= 0x17,
        MBC5 					= 0x19,
        MBC5_RAM 				= 0x1A,
        MBC5_RAM_BATTERY 		= 0x1B,
        MBC5_RUMBLE 			= 0x1C,
        MBC5_RUMBLE_RAM_BATTERY = 0x1E,
        POCKET_CAMERA 			= 0xFC,
        BANDAI_TAMA5 			= 0xFD,
        HuC3 					= 0xFE,
        HuC1_RAM_BATTERY 		= 0xFF
    };
    
    /*! Cartridge's sizes */
    enum class CartridgeSize : byte {
        KB_32	= 0x00,
        KB_64	= 0x01,
        KB_128	= 0x02,
        KB_256	= 0x03,
        KB_512	= 0x04,
        MB_1	= 0x05,
        MB_2	= 0x06,
        MB_4	= 0x07,
        MB_1_1	= 0x52,
        MB_1_2	= 0x53,
        MB_1_5	= 0x54
    };
    
    /*! Destination codes */
    enum class DestinationCode : byte {
        JAPAN		= 0x00,
        NO_JAPAN	= 0x01
    };
    
    /*! RAM sizes */
    enum class RamSize : byte {
        NONE_RAM	= 0x00,
        KB_RAM_2	= 0x01,
        KB_RAM_8	= 0x02,
        KB_RAM_32	= 0x03
    };
    
    /*! Game Color Flag values */
    enum class GameColorFlag : byte {
        CGB_SUPPORT	= 0x80,
        CGB_ONLY	= 0xC0
    };

    /*! Super Game Boy Flag values */
    enum class SuperGameBoyFlag : byte {
        NO_SGB			= 0x00,
        SGB_FUNCTIONS	= 0x03
    };

	Cartridge(std::string);
	~Cartridge();

	std::string getTitle() const;
	DestinationCode getDestinationCode() const;
	CartridgeSize getCartridgeSize() const;
	CartridgeType getCartridgeType() const;
	RamSize getRamSize() const;
	int getROMBanks() const;
	int getRamBanks() const;
	long getRomSize() const;
	void dumpHexadecimalFile() const;
	byte *getRom() const;
	std::string getCartridgeTypeName() const;
	std::string getCartridgeSizeName() const;
	std::string getRamSizeName() const;
	std::string getDestinationCodeName() const;
	std::string getSGBFlagName() const;
	std::string getCGBFlagName() const;
	int getAmountOfRomBanks() const { return romBanks; };
	int getAmountOfRamBanks() const { return ramBanks; };

private:
    byte * rom;
	std::string path;
	std::string title;
    long romSize;
    int romBanks;
    int ramBanks;
    DestinationCode destinationCode;
    CartridgeSize cartridgeSize;
    CartridgeType cartridgeType;
	GameColorFlag cgbFlag;
    SuperGameBoyFlag sgbFlag;
    RamSize ramSize;

    void extractHeaderData();
    void extractRomBanks();
    void extractRamBanks();
    void loadRom();
    long getFileSize(FILE *file);
};

#endif
