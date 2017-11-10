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
    
    static const word LCD_CONTROL = 0xFF40;
    static const word LCDC_STATUS = 0xFF41;
    static const word LY_REGISTER = 0xFF44;
    static const word LY_COMPARE = 0xFF45;
    
    enum Colour : int {
        WHITE,
        LIGHT_GREY,
        DARK_GREY,
        BLACK
    };
    
    Video(Memory*, CPU*);
    ~Video();
    void updateGraphics(short);
    void renderGame();
	void switchPallete();

	void* getFrameBuffer() { return frameBuffer; }

	Colour getColourFromPallete(byte pallete, Colour originalColour);

private:
	const word SCROLL_Y = 0xFF42;
	const word SCROLL_X = 0xFF43;
	const word WINDOWS_Y = 0xFF4A;
	const word WINDOWS_X = 0xFF4B;

	const byte VERTICAL_BLANK_SCAN_LINE = 0x90;
	const byte VERTICAL_BLANK_SCAN_LINE_MAX = 0x99;

	const int MAX_VIDEO_CYCLES = 456;
	const int MIN_HBLANK_MODE_CYCLES = 204;
	const int MIN_OAM_MODE_CYCLES = 80;
	const int MIN_LCD_TRANSFER_CYCLES = 172;
	const int VBLANK_CYCLES = 4560;

	static RGB CLASSIC_PALLETE[4];
	static RGB GREY_PALLETE[4];

	enum class Mode : byte {
		H_BLANK = 0,
		V_BLANK = 1,
		OAM_RAM	= 2,
		LCD_TRANSFER = 3
	};

    RGB frameBuffer[GAMEBOY_HEIGHT][GAMEBOY_WIDTH];
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    CPU *cpu;
    Memory *memory;
    
    Mode mode;
	RGB *pallete;
    int videoCycles;
	int vblankCycles;

    void handleHBlankMode();
    void handleVBlankMode(short cycles);
    void handleOAMMode();
    void handleLCDTransferMode();
    
	bool Video::isLCDEnabled() const {
		return isBitSet(memory->read(LCD_CONTROL), 7);
	}

	void compareLYWithLYC();

    void drawScanline();
    Mode getLCDMode() const;
    void renderBackground(byte);
	void renderSprites(byte);

	inline RGB Video::getColour(const byte colourNumber) const {
		return pallete[colourNumber];
	}

    bool createSDLWindow();
    void resetFrameBuffer();
};
#endif