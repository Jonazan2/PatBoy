#include "Video.h"
#include <map>

Video::Video(Memory *memory, CPU *cpu) {
    this->memory = memory;
    this->cpu = cpu;
    this->createSDLWindow();
    this->resetFrameBuffer();
    this->renderGame();
    this->scanlineCounter = MAX_VIDEO_CYCLES;
}

void Video::updateGraphics(short cycles) {
    
    if (isLCDEnabled()) {
        scanlineCounter -= cycles;
        
        byte currentScanline = memory->readDirectly(LY_REGISTER);
        byte lcdStatus = memory->readDirectly(LCDC_STATUS);
        mode = getLCDMode();

        if (currentScanline <= VERTICAL_BLANK_SCAN_LINE) {
            if (scanlineCounter >= (MAX_VIDEO_CYCLES - MIN_OAM_MODE_CYCLES)) {
                handleOAMMode();
            } else if (scanlineCounter >= (MAX_VIDEO_CYCLES - MIN_OAM_AND_VRAM_MODE_CYCLES - MIN_OAM_MODE_CYCLES)) {
                handleOAMAndVRamMode();
            } else {
                if (mode != 0x0) {
                    clearBit(&lcdStatus, 1);
                    clearBit(&lcdStatus, 0);
                }
                handleHBlankMode();
            }
        } else {
            handleVBlankMode();
        }

        byte lyCompare = memory->readDirectly(LY_COMPARE);
        if (lyCompare == currentScanline) {
            if (isBitSet(lcdStatus, 6)) {
                cpu->requestInterrupt(Interrupts::LCD);
            }
        }
        
    } else {
        scanlineCounter = MAX_VIDEO_CYCLES;
        byte lcdStatus = memory->readDirectly(LCDC_STATUS);
        clearBit(&lcdStatus, 1);
        setBit(&lcdStatus, 0);
        memory->writeDirectly(LY_REGISTER, lcdStatus);
    }
}

void Video::handleHBlankMode() {
    byte lcdStatus = memory->readDirectly(LCDC_STATUS);
    byte scanline = memory->readDirectly(LY_REGISTER);
    memory->writeDirectly(LY_REGISTER, ++scanline);
    
    if (scanline < VERTICAL_BLANK_SCAN_LINE) {
        drawScanline();
    } else {
        clearBit(&lcdStatus, 1);
        setBit(&lcdStatus, 0);
        memory->writeDirectly(LCDC_STATUS, lcdStatus);
        
        if (isBitSet(lcdStatus, 4)) {
            cpu->requestInterrupt(Interrupts::VBLANK);
        }
    }
}

void Video::handleVBlankMode() {
    byte lcdStatus = memory->readDirectly(LCDC_STATUS);
    byte scanline = memory->readDirectly(LY_REGISTER);
    memory->writeDirectly(LY_REGISTER, ++scanline);
    
    if (scanline == VERTICAL_BLANK_SCAN_LINE_MAX) {
        scanlineCounter = MAX_VIDEO_CYCLES;
        memory->writeDirectly(LY_REGISTER, 0x0);
        setBit(&lcdStatus, 1);
        clearBit(&lcdStatus, 0);
        memory->writeDirectly(LCDC_STATUS, lcdStatus);
        
        
        if (isBitSet(lcdStatus, 5)) {
            cpu->requestInterrupt(Interrupts::LCD);
        }
    }
}

void Video::handleOAMMode() {
    byte lcdStatus = memory->readDirectly(LCDC_STATUS);
    setBit(&lcdStatus, 1);
    clearBit(&lcdStatus, 0);
    memory->writeDirectly(LCDC_STATUS, lcdStatus);
    
    if (isBitSet(lcdStatus, 3)) {
        cpu->requestInterrupt(Interrupts::LCD);
    }

}

void Video::handleOAMAndVRamMode() {
    byte lcdStatus = memory->readDirectly(LCDC_STATUS);
    setBit(&lcdStatus,1);
    setBit(&lcdStatus,0);
    memory->writeDirectly(LCDC_STATUS, lcdStatus);
}

void Video::drawScanline() {
    const byte lcdControl = memory->readDirectly(LCD_CONTROL);
	if ( isBitSet(lcdControl, 0) ) {
		renderBackground(lcdControl);
    } else {
        renderSprites(lcdControl);
    }
}

void Video::renderBackground(byte lcdControl) {
    word tileData;
    word backgroundMemory;
    bool unsig = true;
    bool usingWindow = false;

    byte scrollY = memory->readDirectly(SCROLL_Y);
    byte scrollX = memory->readDirectly(SCROLL_X);
    byte windowY = memory->readDirectly(WINDOWS_Y);
    byte windowX = memory->readDirectly(WINDOWS_X) - 7;

	const int scanline = memory->readDirectly(LY_REGISTER);

    if ( isBitSet(lcdControl, 5) ) {
		usingWindow = windowY <= scanline;
    }

    if ( isBitSet(lcdControl,4) ) {
        tileData = 0x8000;
    } else {
        tileData = 0x8800;
        unsig= false;
    }

    if ( !usingWindow ) {
        if ( isBitSet(lcdControl, 3) ) {
            backgroundMemory = 0x9C00;
        } else {
            backgroundMemory = 0x9800;
        }
    } else {
        if ( isBitSet(lcdControl, 6) ) {
            backgroundMemory = 0x9C00;
        } else {
            backgroundMemory = 0x9800;
        }
    }


    byte yPos = 0;
    if ( !usingWindow ) {
        yPos = scrollY + scanline;
    } else {
        yPos = scanline - windowY;
    }

    word tileRow = (((byte)(yPos/8))*32);

    for ( int pixel = 0 ; pixel < 160; pixel++ ) {
        byte xPos = pixel + scrollX;

        if ( usingWindow && pixel >= windowX ) {
			xPos = pixel - windowX;
        }

        const word tileCol = (xPos/8);
        signed short tileNum;
        
        if( unsig ) {
            tileNum = (byte)memory->readDirectly(backgroundMemory+tileRow + tileCol);
        } else {
            tileNum = (signed short)memory->readDirectly(backgroundMemory+tileRow + tileCol);
        }

        word tileLocation = tileData;
        if ( unsig ) {
            tileLocation += (tileNum * 16);
        } else {
            tileLocation += ((tileNum+128) *16);
        }

        byte line = yPos % 8;
        line *= 2;
        byte data1 = memory->readDirectly(tileLocation + line);
        byte data2 = memory->readDirectly(tileLocation + line + 1);

        int colourBit = xPos % 8;
        colourBit -= 7;
        colourBit *= -1;
        
        int colourNum = getBitValue(data2, colourBit);
        colourNum <<= 1;
        colourNum |= getBitValue(data1,colourBit);

        RGB colour = getColour(colourNum);

        frameBuffer[scanline][pixel][0] = colour.red;
        frameBuffer[scanline][pixel][1] = colour.green;
        frameBuffer[scanline][pixel][2] = colour.blue;
    }
}

void Video::renderSprites(byte lcdControl) {
    bool use8x16 = false;

    if ( isBitSet(lcdControl, 2) ) {
        use8x16 = true;
    }

    for (int sprite = 0 ; sprite < 40; sprite++) {
        byte index = sprite*4 ;
        byte yPos = memory->read(0xFE00+index) - 16;
        byte xPos = memory->read(0xFE00+index+1)-8;
        byte tileLocation = memory->read(0xFE00+index+2) ;
        byte attributes = memory->read(0xFE00+index+3) ;

        bool yFlip =  isBitSet(attributes, 6);
        bool xFlip = isBitSet(attributes, 5);
        bool hidden = isBitSet(attributes, 7);

        int scanline = memory->read(LY_REGISTER);

        int ysize = 8;

        if ( use8x16 ) {
            ysize = 16;
        }

        if ( (scanline >= yPos) && (scanline < (yPos+ysize)) ) {
            int line = scanline - yPos;

            if ( yFlip ) {
                line -= ysize;
                line *= -1;
            }

            line *= 2;
            byte data1 = memory->read((0x8000 + (tileLocation * 16)) + line);
            byte data2 = memory->read((0x8000 + (tileLocation * 16)) + line+1);

            for ( int tilePixel = 7; tilePixel >= 0; tilePixel-- ) {
                int colourbit = tilePixel ;
                if ( xFlip ) {
                    colourbit -= 7;
                    colourbit *= -1;
                }

                int colourNum = getBitValue(data2,colourbit) ;
                colourNum <<= 1;
                colourNum |= getBitValue(data1,colourbit) ;

				RGB colour = getColour(colourNum);

                int xPix = 0 - tilePixel;
                xPix += 7;

                int pixel = xPos+xPix;

                if ( hidden && frameBuffer[scanline][pixel][0] == white ) {
                    hidden = false;
                }

                if ( !hidden ){
                    frameBuffer[scanline][pixel][0] = colour.red;
                    frameBuffer[scanline][pixel][1] = colour.green;
                    frameBuffer[scanline][pixel][2] = colour.blue;
                }
 			}
		}
	}
}

byte Video::getLCDMode() const {
    byte lcdStatus = memory->readDirectly(LCDC_STATUS);
	return lcdStatus & 0x3;
}


bool Video::createSDLWindow() {
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
        return false;
    }
    
    window = SDL_CreateWindow("PatBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              GAMEBOY_WIDTH, GAMEBOY_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                SDL_TEXTUREACCESS_STREAMING, GAMEBOY_WIDTH, GAMEBOY_HEIGHT);
    return true ;
}

void Video::renderGame() {
    SDL_UpdateTexture(texture, NULL, this->frameBuffer, GAMEBOY_WIDTH * sizeof(byte) * 3);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Video::resetFrameBuffer() {
	memset(frameBuffer, 255, sizeof(frameBuffer));
}

Video::~Video() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

