#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include <iostream>
#include <SDL2/SDL.h>
#include "Memory.h"
#include "CPU.h"

class Memory;
class CPU;

/**************************************************************************//**
 * \brief Class that models the joypad system of the Nintendo Game Boy
 *
 * The Joypad has a state modeled by a single byte and two fundamental methods:
 * keyPressed and keyReleased that are called when a pulsation occurs in the keyboard.
 * It has access to the memory and cpu instance as well in order to request interrupts
 * and update its register.
 *
 * See also \ref CPU \ref Memory \ref Utils
******************************************************************************/
class Joypad {
    
private:
    Memory *memory;
    CPU *cpu;
    byte joypadState;
    void keyPressed(int);
    void keyReleased(int);
    
public:
    Joypad();
    void init(CPU *, Memory*);
    void handleInput(SDL_Event &);
    byte getState();
    
};

#endif
