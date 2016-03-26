#include "GameBoy.h"

const int MAX_CYCLES = 70224;
const float FPS = 59.7;
const float DELAY_TIME = 1000.0f / FPS;

GameBoy::GameBoy(const string path) {
    this->audio = new Audio();
    this->audio->reset();
    this->cartridge = new Cartridge(path);
    this->joypad = new Joypad();
    MemoryFactory *factory = new MemoryFactory();
    this->memory = factory->createMemoryChip(cartridge, audio, joypad);
    this->cpu = new CPU(memory);
    this->memory->init(cpu);
    joypad->init(cpu, memory);
    this->video = new Video(memory, cpu);
}

void GameBoy::startEmulation() {
    bool quit = false;
	SDL_Event event;
    
	int cycles;
	
	std::chrono::time_point<std::chrono::system_clock> current, previous;
	previous = std::chrono::system_clock::now();

    while (!quit) {
		current = std::chrono::system_clock::now();
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
		}
		video->renderGame();

		if (cartridge->hasRTC()) {
			cartridge->updateRTC();
		}

		if (elapsed.count() < DELAY_TIME) {
			int waitTime = (int)(DELAY_TIME - elapsed.count());
			std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
		} else {
			printf("Time overpass: %f\n", elapsed.count() - DELAY_TIME);
		}
    }
	SDL_Quit( ) ;
}

GameBoy::~GameBoy() {
    delete cartridge;
    delete audio;
    delete joypad;
    delete memory;
    delete video;
    delete cpu;
}