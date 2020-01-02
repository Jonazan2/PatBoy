#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "Types.h"
#include "Memory.h"
#include "CPU.h"
#include "Utils.h"

#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif __linux__
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
        WHITE = 0,
        LIGHT_GREY,
        DARK_GREY,
        BLACK
    };

	enum class Mode : byte {
		H_BLANK = 0,
		V_BLANK = 1,
		OAM_RAM = 2,
		LCD_TRANSFER = 3
	};

    
    Video(Memory*, CPU*);
    ~Video();

	void reset();

    void updateGraphics(short);
    void renderGame();
	void switchPallete();
	void changeWindowSize();
	void DisableLCD();

	void* getFrameBuffer() { return frameBuffer; }
	RGB* const getCurrentPallete() const { return currentPallete; }
	Colour getColourFromPallete(byte pallete, Colour originalColour);
	Video::Mode getCurrentMode() const { return mode; }

	bool isLCDEnabled() const { return lcdEnabled; }
	void EnableLCD() { lcdEnabled = true; }
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

	struct WindowResolution
	{
		int width;
		int height;

		bool operator==(const WindowResolution& rhs) const {
			return width == rhs.width && height == rhs.height;
		}
	};
	static const int MAX_AMOUNT_WINDOW_RESOLUTIONS = 3;
	static const WindowResolution FULLSCREEN_WINDOW;
	static const WindowResolution WINDOW_RESOLUTIONS[MAX_AMOUNT_WINDOW_RESOLUTIONS];
	
    RGB frameBuffer[GAMEBOY_HEIGHT * GAMEBOY_WIDTH];
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
	int currentResolutionIndex;
    CPU *cpu;
    Memory *memory;
	void *icon;

	bool lcdEnabled;
    Mode mode;
	RGB *currentPallete;
    int videoCycles;
	int vblankCycles;

    void handleHBlankMode();
    void handleVBlankMode(short cycles);
    void handleOAMMode();
    void handleLCDTransferMode();


	void compareLYWithLYC();

    void drawScanline();
    Mode getLCDMode() const;
	void renderBackground(byte);
	void renderWindow(byte, byte, byte);
	void renderTile(bool unsig, word tileMap, word tileData, byte xPos, byte yPos, byte pixel, byte backgroundPallete, byte scanline);
	void renderSprites(byte);

	inline RGB getColour(const byte colourNumber) const { return currentPallete[colourNumber]; }

    bool createSDLWindow();
	bool setWindowIcon();
    void resetFrameBuffer();
};

#endif