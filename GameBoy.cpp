#include "GameBoy.h"

const int MAX_CYCLES = 70221;
const int FPS = 59.7;
const int DELAY_TIME = 1000.0f / FPS;

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
    
    int frameTime, frameStart;
    while (!quit) {
    
        frameStart = SDL_GetTicks();
        
		while( SDL_PollEvent( &event ) ) {
            joypad->handleInput(event);
            
			if( event.type == SDL_QUIT ) {
				quit = true;
			}
		}
		this->update();

        frameTime = SDL_GetTicks() - frameStart;
        
        if(frameTime < DELAY_TIME){
            SDL_Delay(DELAY_TIME - frameTime);
        } else {
            printf("Time overpass: %d\n", frameTime - DELAY_TIME);
        }
    }
	SDL_Quit( ) ;
}


void GameBoy::update() {
    int cycles = 0;
    while ( cycles < MAX_CYCLES ) {
        short cyclesPerThisOpcode = cpu->update();
        cycles += cyclesPerThisOpcode;
        cpu->updateTimers(cyclesPerThisOpcode);
        video->updateGraphics(cyclesPerThisOpcode);
        audio->update(cyclesPerThisOpcode);
        cpu->updateInterrupts();
    }
    video->renderGame();
    
    if ( cartridge->hasRTC() ) {
        cartridge->updateRTC();
    }
}

GameBoy::~GameBoy() {
    delete cartridge;
    delete audio;
    delete joypad;
    delete memory;
    delete video;
    delete cpu;
}