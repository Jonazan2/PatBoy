#ifndef _GAMEBOY_H_
#define _GAMEBOY_H_

#include <string>
#include "Cartridge.h"
#include "Memory.h"
#include "Video.h"
#include "CPU.h"
#include "Types.h"
#include "MemoryFactory.h"
#include "Audio.h"
#include "Joypad.h"
#include <iostream>
#include <thread>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

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
    
private:
    Audio *audio;
    Cartridge *cartridge;
    Memory *memory;
    CPU *cpu;
    Video *video;
    Joypad *joypad;
    
public:
    GameBoy(const string);
    void startEmulation();
    ~GameBoy();
};

#endif