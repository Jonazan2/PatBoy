#include "Joypad.h"

Joypad::Joypad(){
    joypadState = 0xFF;
}

void Joypad::init(CPU *cpu, Memory *memory){
    this->cpu = cpu;
    this->memory = memory;
}

void Joypad::keyPressed(int key) {
    bool previouslyUnset = false ;
    
	if ( !isBitSet(joypadState, key) ) {
		previouslyUnset = true ;
    }
    
    clearBit(&joypadState, key);
    
	bool button;
    
	if ( key > 3 ) {
		button = true ;
    } else {
		button = false ;
    }
    
	byte keyReq = memory->readDirectly(0xFF00);
	bool requestInterupt = false;
    
	if ( button && !isBitSet(keyReq,5) ) {
		requestInterupt = true ;
	} else if ( !button && !isBitSet(keyReq,4) ) {
		requestInterupt = true ;
	}
    
	if ( requestInterupt && !previouslyUnset ) {
        cpu->requestInterrupt(CPU::JOYPAD);
	}
}

void Joypad::keyReleased(int key) {
    setBit(&joypadState, key);
}

byte Joypad::getState() {
	byte res = memory->readDirectly(0xFF00);
	res ^= 0xFF ;
    
	if ( !isBitSet(res, 4) ) {
		byte topJoypad = joypadState >> 4 ;
		topJoypad |= 0xF0 ;
		res &= topJoypad ;
	} else if ( !isBitSet(res,5) ) {
		byte bottomJoypad = joypadState & 0xF ;
		bottomJoypad |= 0xF0 ;
		res &= bottomJoypad ;
	}
	return res ;
}

void Joypad::handleInput(SDL_Event& event) {
	if( event.type == SDL_KEYDOWN ) {
		int key = -1 ;
        
		switch( event.key.keysym.sym ) {
			case SDLK_z:        key = 4;    break;
			case SDLK_x:        key = 5;    break;
			case SDLK_RETURN:   key = 7;    break;
			case SDLK_SPACE:    key = 6;    break;
			case SDLK_RIGHT:    key = 0;    break;
			case SDLK_LEFT:     key = 1;    break;
			case SDLK_UP:       key = 2;    break;
			case SDLK_DOWN:     key = 3;    break;
            default:            key = -1;   break;
		}
        
		if ( key != -1 ) {
			keyPressed(key) ;
		}
	} else if( event.type == SDL_KEYUP ) {
		int key = -1 ;
        
		switch( event.key.keysym.sym ) {
			case SDLK_z:        key = 4;    break;
			case SDLK_x:        key = 5;    break;
			case SDLK_RETURN:   key = 7;    break;
			case SDLK_SPACE:    key = 6;    break;
			case SDLK_RIGHT:    key = 0;    break;
			case SDLK_LEFT:     key = 1;    break;
			case SDLK_UP:       key = 2;    break;
			case SDLK_DOWN:     key = 3;    break;
            default:            key = -1;   break;
		}
        
		if ( key != -1 ) {
			keyReleased(key) ;
		}
	}
}