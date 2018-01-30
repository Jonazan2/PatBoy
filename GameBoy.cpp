#include "GameBoy.h"

#include <iostream>
#include <thread>


#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

GameBoy::GameBoy(const std::string &path) {
    this->audio = new Audio();
    this->audio->reset();
    this->cartridge = new Cartridge(path);
    this->joypad = new Joypad();
	this->memory = new Memory(cartridge, audio, joypad);
    this->cpu = new CPU(memory);
    this->memory->init(cpu);
    joypad->init(cpu, memory);
    this->video = new Video(memory, cpu);
	this->debugger = new Debugger(cpu, memory, video, cartridge);
}

void GameBoy::startEmulation() {
	debugger->startDebugger();
  
	int cycles;
	std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
	previous = std::chrono::high_resolution_clock::now();

	SDL_Event event;
    bool quit = false;
    while (!quit) {
		current = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (current - previous);
		previous = current;
		
		while( SDL_PollEvent( &event ) ) {
            joypad->handleInput(event);
            
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_F2) {
					video->switchPallete();
				}
			}

			if( event.type == SDL_QUIT ) {
				quit = true;
			}
		}

		cycles = 0;
		while (cycles < MAX_CYCLES) {
			short cyclesPerThisOpcode = cpu->update();
			cpu->updateTimers(cyclesPerThisOpcode);
			video->updateGraphics(cyclesPerThisOpcode);
			audio->update(cyclesPerThisOpcode);
			cpu->updateInterrupts(cyclesPerThisOpcode);
			cycles += cyclesPerThisOpcode;
			debugger->update(cycles);
		}
		video->renderGame();

		if (elapsed.count() < DELAY_TIME) {
			std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(DELAY_TIME - elapsed.count()));
		} else {
			printf("overpassed by %f\n", elapsed.count() - DELAY_TIME);
		}
    }

	SDL_Quit();
	debugger->closeDebugger();
}

GameBoy::~GameBoy() {
    delete audio;
    delete joypad;
    delete memory;
    delete cartridge;
    delete video;
    delete cpu;
	delete debugger;
}
