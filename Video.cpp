#include "Video.h"

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
        byte lcdStatus = memory->readDirectly(LCD_STATUS);
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
        
        byte lyRegister = memory->readDirectly(LY_REGISTER);
        byte lyCompare = memory->readDirectly(LY_COMPARE);
        if (lyCompare == lyRegister) {
            if (isBitSet(lcdStatus, 6)) {
                cpu->requestInterrupt(Interrupts::LCD);
            }
        }
        
    } else {
        scanlineCounter = MAX_VIDEO_CYCLES;
        byte lcdStatus = memory->readDirectly(LCD_STATUS);
        clearBit(&lcdStatus, 1);
        setBit(&lcdStatus, 0);
        memory->writeDirectly(LY_REGISTER, lcdStatus);
    }
}

void Video::handleHBlankMode() {
    byte lcdStatus = memory->readDirectly(LCD_STATUS);
    byte scanline = memory->readDirectly(LY_REGISTER);
    memory->writeDirectly(LY_REGISTER, ++scanline);
    
    if (scanline < VERTICAL_BLANK_SCAN_LINE) {
        drawScanline();
    } else {
        clearBit(&lcdStatus, 1);
        setBit(&lcdStatus, 0);
        memory->writeDirectly(LCD_STATUS, lcdStatus);
        
        if (isBitSet(lcdStatus, 4)) {
            cpu->requestInterrupt(Interrupts::VBLANK);
        }
    }
}

void Video::handleVBlankMode() {
    byte lcdStatus = memory->readDirectly(LCD_STATUS);
    byte scanline = memory->readDirectly(LY_REGISTER);
    memory->writeDirectly(LY_REGISTER, ++scanline);
    
    if (scanline == VERTICAL_BLANK_SCAN_LINE_MAX) {
        scanlineCounter = MAX_VIDEO_CYCLES;
        memory->writeDirectly(LY_REGISTER, 0x0);
        setBit(&lcdStatus, 1);
        clearBit(&lcdStatus, 0);
        memory->writeDirectly(LCD_STATUS, lcdStatus);
        
        
        if (isBitSet(lcdStatus, 5)) {
            cpu->requestInterrupt(Interrupts::LCD);
        }
    }
}

void Video::handleOAMMode() {
    byte lcdStatus = memory->readDirectly(LCD_STATUS);
    setBit(&lcdStatus, 1);
    clearBit(&lcdStatus, 0);
    memory->writeDirectly(LCD_STATUS, lcdStatus);
    
    if (isBitSet(lcdStatus, 3)) {
        cpu->requestInterrupt(Interrupts::LCD);
    }

}

void Video::handleOAMAndVRamMode() {
    byte lcdStatus = memory->readDirectly(LCD_STATUS);
    setBit(&lcdStatus,1);
    setBit(&lcdStatus,0);
    memory->writeDirectly(LCD_STATUS, lcdStatus);
}

bool Video::isLCDEnabled() const {
    return isBitSet(memory->read(LCDS_CONTROL), 7);
}

void Video::drawScanline() {
    byte lcdControl = memory->readDirectly(LCDS_CONTROL);
	if ( isBitSet(lcdControl, 0) ) {
		renderBackground(lcdControl);
    } else if ( isBitSet(lcdControl, 1) ) {
        renderSprites(lcdControl);
    }
}

void Video::renderBackground(byte lcdControl) {
    word tileData = 0 ;
    word backgroundMemory =0 ;
    bool unsig = true ;

    byte scrollY = memory->read(SCROLL_Y);
    byte scrollX = memory->read(SCROLL_X);
    byte windowY = memory->read(WINDOWS_Y);
    byte windowX = memory->read(WINDOWS_X) - 7;
    bool usingWindow = false;

    if ( isBitSet(lcdControl, 5) ) {
        if (windowY <= memory->read(LY_REGISTER) ) {
            usingWindow = true;
        }
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
        yPos = scrollY + memory->read(LY_REGISTER);
    } else {
        yPos = memory->read(LY_REGISTER) - windowY;
    }

    word tileRow = (((byte)(yPos/8))*32);

    for ( int pixel = 0 ; pixel < 160; pixel++ ) {
        byte xPos = pixel+scrollX;

        if ( usingWindow ) {
            if ( pixel >= windowX ) {
                xPos = pixel - windowX;
            }
        }

        word tileCol = (xPos/8);
        signed short tileNum;
        
        if( unsig ) {
            tileNum = (byte)memory->read(backgroundMemory+tileRow + tileCol);
        } else {
            tileNum = (signed short)memory->read(backgroundMemory+tileRow + tileCol);
        }

        word tileLocation = tileData;
        if ( unsig ) {
            tileLocation += (tileNum * 16);
        } else {
            tileLocation += ((tileNum+128) *16);
        }

        byte line = yPos % 8;
        line *= 2;
        byte data1 = memory->read(tileLocation + line);
        byte data2 = memory->read(tileLocation + line + 1);

        int colourBit = xPos % 8;
        colourBit -= 7;
        colourBit *= -1;
        
        int colourNum = getBitValue(data2, colourBit);
        colourNum <<= 1;
        colourNum |= getBitValue(data1,colourBit);

        Colour colour = getColour(colourNum, 0xFF47);
        RGB pixelColours = getPixelWithColour(colour);

        int scanline = memory->read(LY_REGISTER);

        frameBuffer[scanline][pixel][0] = pixelColours.red;
        frameBuffer[scanline][pixel][1] = pixelColours.green;
        frameBuffer[scanline][pixel][2] = pixelColours.blue;
    }
}

RGB Video::getPixelWithColour(const Colour colour) {
    RGB rgb;
    switch( colour ) {
        case white:	    rgb.red = 255;  rgb.green = 255;  rgb.blue = 255;  break;
        case lightGray: rgb.red = 0xCC; rgb.green = 0xCC; rgb.blue = 0xCC; break;
        case darkGray:	rgb.red = 0x77; rgb.green = 0x77; rgb.blue = 0x77; break;
        case black:     rgb.red = 0x0;  rgb.green = 0x0;  rgb.blue = 0x0;  break;
    }
    return rgb;
}

Video::Colour Video::getColour(const byte colourNumber, const word address) const {
	Colour res = white;
	byte palette = memory->readDirectly(address);
	int hi = 0;
	int lo = 0;

	switch ( colourNumber ) {
        case 0: hi = 1; lo = 0; break;
        case 1: hi = 3; lo = 2; break;
        case 2: hi = 5; lo = 4; break;
        case 3: hi = 7; lo = 6; break;
	}

	int colour = 0;
	colour = getBitValue(palette, hi) << 1;
	colour |= getBitValue(palette, lo) ;

	switch ( colour ) {
        case 0: res = white;       break;
        case 1: res = lightGray;   break;
        case 2: res = darkGray;    break;
        case 3: res = black;       break;
	}

	return res;
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

                Colour colour;
                if ( isBitSet(attributes, 4) ) {
                    colour = getColour(colourNum, 0xFF49);
                } else {
                    colour = getColour(colourNum, 0xFF48);
                }

                
                int xPix = 0 - tilePixel;
                xPix += 7;

                int pixel = xPos+xPix;

                if ( hidden && frameBuffer[scanline][pixel][0] == white ) {
                    hidden = false;
                }

                if ( !hidden ){
                    RGB pixelColour = getPixelWithColour(colour);
                    frameBuffer[scanline][pixel][0] = pixelColour.red;
                    frameBuffer[scanline][pixel][1] = pixelColour.green;
                    frameBuffer[scanline][pixel][2] = pixelColour.blue;
                }
 			}
		}
	}
}

byte Video::getLCDMode() const {
    byte lcdStatus = memory->readDirectly(LCD_STATUS);
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
    for ( int x = 0 ; x < 144; x++ ) {
        for ( int y = 0; y < 160; y++ ) {
            frameBuffer[x][y][0] = 255;
            frameBuffer[x][y][1] = 255;
            frameBuffer[x][y][2] = 255;
        }
    }
}

void Video::printVideoRegistersState() const {
    printf("LCDS CONTROL: %02X\t", memory->readDirectly(LCDS_CONTROL));
    printf("LCD STATUS: %02X\t", memory->readDirectly(LCD_STATUS));
    printf("LY_REGISTER: %02X\t" , memory->readDirectly(LY_REGISTER));
    printf("LY_COMPARE: %02X\t", memory->readDirectly(LY_COMPARE));
    printf("SCROLL X: %02X\t", memory->readDirectly(SCROLL_X));
    printf("SCROLL Y: %02X\n\n", memory->readDirectly(SCROLL_Y));
}

