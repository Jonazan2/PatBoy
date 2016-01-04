#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "Types.h"
#include "Memory.h"
#include "CPU.h"
#include "Utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

/**************************************************************************//**
 * \brief Class that implements the Video system of the Nintendo Game Boy
 *
 * This class implements the creation of the SDL window and the rendering with OpenGL.
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
    const int RETRACE_START = 456;
    
    const word LCD_CONTROL = 0xFF40;
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
    void updateGraphics(short);
    void renderGame();
    
private:
    
    byte frameBuffer[GAMEBOY_HEIGHT][GAMEBOY_WIDTH][3];
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    CPU *cpu;
    Memory *memory;
    
    int scanlineCounter;
    byte currentScanline;
    
    void updateRegisterLCD();
    bool isLCDEnabled() const;
    void drawCurrentScanline();
    void drawScanline();
    byte getLCDMode() const;
    void renderBackground(byte);
    void renderSprites(byte);
    Colour getColour(const byte, const word) const;
    bool createSDLWindow();
    void initializeOpenGL();
    void resetFrameBuffer();
};
#endif