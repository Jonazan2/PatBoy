#pragma once

#include <string>
#include "Cartridge.h"
#include "Memory.h"
#include "Video.h"
#include "CPU.h"
#include "Types.h"
#include "Audio.h"
#include "Joypad.h"
#include "Debugger/Debugger.h"

/**************************************************************************//**
 * \brief Class that models a Game Boy with all its systems.
 *
 * This class implements a Game Boy at the design level. Contains a pointer 
 * to each system that make the console. It contains the main loop emulation.
 *
 * See also \ref Audio \ref Cartridge \ref CPU \ref Video \ref Joypad
 *
******************************************************************************/

class GameBoy {

public:
	GameBoy(const std::string&);
	void startEmulation();
	~GameBoy();
    
private:
	const int MAX_CYCLES = 70224;
	const float FPS = 59.73f;
	const float DELAY_TIME = 1000.0f / FPS;

    Audio *audio;
    Cartridge *cartridge;
    Memory *memory;
    CPU *cpu;
    Video *video;
    Joypad *joypad;
	Debugger *debugger;
};