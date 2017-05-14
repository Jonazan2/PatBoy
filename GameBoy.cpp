#include "GameBoy.h"

#include <iostream>
#include <thread>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

const int MAX_CYCLES = 70224;
const float FPS = 59.7f;
const float DELAY_TIME = 1000.0f / FPS;

GameBoy::GameBoy(const std::string path) {
    this->audio = new Audio();
    this->audio->reset();
    this->cartridge = new Cartridge(path);
    this->joypad = new Joypad();
	this->memory = new Memory(cartridge, audio, joypad);
    this->cpu = new CPU(memory);
    this->memory->init(cpu);
    joypad->init(cpu, memory);
    this->video = new Video(memory, cpu);
}

void GameBoy::startEmulation() {
	debugger.startDebugger(*cpu, *memory, *video);

    bool quit = false;
	SDL_Event event;
  
	int cycles;
	
	std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
	previous = std::chrono::high_resolution_clock::now();

    while (!quit) {
		current = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> (current - previous);
		previous = current;
		
		while( SDL_PollEvent( &event ) ) {
            joypad->handleInput(event);
            
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
			cpu->updateInterrupts();
			cycles += cyclesPerThisOpcode;
			debugger.update(cycles, *cpu, *memory, *video);
		}
		video->renderGame();

		if (cartridge->hasRTC()) {
			cartridge->updateRTC();
		}

		if (elapsed.count() < DELAY_TIME) {
			int waitTime = (int)(DELAY_TIME - elapsed.count());
			std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
		} else {
			printf("overpassed by %f\n", elapsed.count() - DELAY_TIME);
		}
    }
	SDL_Quit( ) ;
	debugger.closeDebugger();
}

GameBoy::~GameBoy() {
    delete cartridge;
    delete audio;
    delete joypad;
    delete memory;
    delete video;
    delete cpu;
}
