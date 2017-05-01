#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "Types.h"
#include "Memory.h"
#include "CPU.h"
#include "Utils.h"

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

/**************************************************************************//**
 * \brief Class that implements the Video system of the Nintendo Game Boy
 *
 * This class implements the creation of the SDL window and the rendering with SDL2.
 * It contains methods to render the sprites and tiles as well as methods to control
 * and update the registers of the video system.
 *
 * See also \ref CPU \ref Memmory \ref Utils                                                                            
******************************************************************************/

class Video {
    
public:
    
    static const short GAMEBOY_WIDTH = 160;
    static const short GAMEBOY_HEIGHT = 144;
    
    const word SCROLL_Y = 0xFF42;
    const word SCROLL_X = 0xFF43;
    const word WINDOWS_Y = 0xFF4A;
    const word WINDOWS_X = 0xFF4B;
    
    const byte VERTICAL_BLANK_SCAN_LINE = 0x90;
    const byte VERTICAL_BLANK_SCAN_LINE_MAX = 0x99;
    
    const short HBLANK_MODE = 0x0;
    const short VBLANK_MODE = 0x1;
    const short OAM_MODE = 0x02;
    const short OAM_VRAM_MODE = 0x03;
    
    const int MAX_VIDEO_CYCLES = 456;
    const int MIN_HBLANK_MODE_CYCLES = 201;
    const int MIN_OAM_MODE_CYCLES = 77;
    const int MIN_OAM_AND_VRAM_MODE_CYCLES = 169;
    
    const word LCDS_CONTROL = 0xFF40;
    const word LCD_STATUS = 0xFF41;
    const word LY_REGISTER = 0xFF44;
    const word LY_COMPARE = 0xFF45;
    
    enum Colour {
        white,
        lightGray,
        darkGray,
        black
    };
    
    Video(Memory*, CPU*);
    ~Video();
    void updateGraphics(short);
    void renderGame();
    
private:

    byte frameBuffer[GAMEBOY_HEIGHT][GAMEBOY_WIDTH][3];
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    CPU *cpu;
    Memory *memory;
    
    short mode;
    int scanlineCounter;
    byte currentScanline;
    
    void handleHBlankMode();
    void handleVBlankMode();
    void handleOAMMode();
    void handleOAMAndVRamMode();
    
	bool Video::isLCDEnabled() const {
		return isBitSet(memory->read(LCDS_CONTROL), 7);
	}

    void drawScanline();
    byte getLCDMode() const;
    void renderBackground(byte);
	void renderSprites(byte);

	inline RGB Video::getColour(const byte colourNumber) const {
		static RGB white = { 255,255,255 };
		static RGB lightGray = { 0xCC,0xCC,0xCC };
		static RGB darkGray = { 0x77,0x77,0x77 };
		static RGB black = { 0x0,0x0,0x0 };

		switch (colourNumber) {
		case Colour::white:
			return white;
		case Colour::lightGray:
			return lightGray;
		case Colour::darkGray:
			return darkGray;
		case Colour::black:
		default:
			return black;
		}
	}

    bool createSDLWindow();
    void resetFrameBuffer();
    void printVideoRegistersState() const;
};
#endif