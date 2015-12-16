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
    
    enum Colour {
        white,
        lightGray,
        darkGray,
        black
    };
    
private:
    
    byte frameBuffer[GAMEBOY_HEIGHT][GAMEBOY_WIDTH][3];
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    CPU *cpu;
    Memory *memory;
    Utils *utils;
    int scanlineCounter;
    byte currentScanline;
    
    void updateRegisterLCD();
    bool isLCDEnabled();
    void drawCurrentScanline();
    void drawScanline();
    byte getLCDMode() const;
    void renderBackground(byte);
    void renderSprites(byte);
    Colour getColour(const byte, const word) const;
    bool createSDLWindow();
    void initializeOpenGL();
    void resetFrameBuffer();
    
public:
    Video(Memory*, CPU*);
    void updateGraphics(short);
    void renderGame();
};
#endif