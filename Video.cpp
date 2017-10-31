#include "Video.h"
#include <map>

RGB Video::CLASSIC_PALLETE[4] = { { 155,188,15 }, { 139,172,15 }, { 48,98,48 }, { 15,56,15 } };
RGB Video::GREY_PALLETE[4] = { { 255,255,255 },{ 0xCC,0xCC,0xCC },{ 0x77,0x77,0x77 }, { 0x0,0x0,0x0 } };

Video::Video(Memory *memory, CPU *cpu) {
    this->memory = memory;
    this->cpu = cpu;
    this->createSDLWindow();
    this->resetFrameBuffer();
    this->renderGame();
    
	this->videoCycles = 0;
	this->vblankCycles = 0;
	this->mode = Video::Mode::OAM_RAM;
	this->pallete = GREY_PALLETE;
}

void Video::updateGraphics(short cycles) {
    if (isLCDEnabled()) {
		videoCycles += cycles;
		switch (mode) {
			case Video::Mode::H_BLANK:
				handleHBlankMode();
				break;
			case Video::Mode::V_BLANK:
				handleVBlankMode(cycles);
				break;
			case Video::Mode::OAM_RAM:
				handleOAMMode();
				break;
			case Video::Mode::LCD_TRANSFER:
				handleLCDTransferMode();
				break;
		}
	} else {
		videoCycles = 0;
		memory->writeDirectly(LY_REGISTER, 0);

		byte lcdc = memory->readDirectly(LCDC_STATUS);
		lcdc &= 0xFC;
		memory->writeDirectly(LCDC_STATUS, lcdc);
		mode = Video::Mode::H_BLANK;

		compareLYWithLYC();
	}
}

void Video::handleHBlankMode() {
    if (videoCycles >= MIN_HBLANK_MODE_CYCLES) {
		videoCycles = 0;

		byte scanline = memory->readDirectly(LY_REGISTER);
		memory->writeDirectly(LY_REGISTER, ++scanline);
		compareLYWithLYC();
		if (scanline == VERTICAL_BLANK_SCAN_LINE) {
			mode = Mode::V_BLANK;

			/* Move to mode 1: VBLANK*/
			byte lcdStatus = memory->readDirectly(LCDC_STATUS);
			setBit(&lcdStatus, 1);
			memory->writeDirectly(LCDC_STATUS, lcdStatus);
			cpu->requestInterrupt(Interrupts::VBLANK);
		} else {
			mode = Mode::OAM_RAM;
		}
    }
}

void Video::handleVBlankMode(short cycles) {
	vblankCycles += cycles;

    if (vblankCycles >= MAX_VIDEO_CYCLES) {
		vblankCycles = 0;

		byte scanline = memory->readDirectly(LY_REGISTER);
		if (scanline < VERTICAL_BLANK_SCAN_LINE_MAX) {
			memory->writeDirectly(LY_REGISTER, ++scanline);
		}
		compareLYWithLYC();
	}

	if (videoCycles >= VBLANK_CYCLES) {
		videoCycles = 0;
		vblankCycles = 0;

		/* Reset scanlines to zero */
		memory->writeDirectly(LY_REGISTER, 0);
		compareLYWithLYC();

		/* Move to mode 2: OAM Mode*/
		mode = Mode::OAM_RAM;
		byte lcdStatus = memory->readDirectly(LCDC_STATUS);
        setBit(&lcdStatus, 1);
        clearBit(&lcdStatus, 0);
        memory->writeDirectly(LCDC_STATUS, lcdStatus);

        if (isBitSet(lcdStatus, 5)) {
            cpu->requestInterrupt(Interrupts::LCD);
        }
    }
}

void Video::handleOAMMode() {
	if (videoCycles >= MIN_OAM_MODE_CYCLES) {
		videoCycles = 0;

		/* Move to mode 3 */
		mode = Mode::LCD_TRANSFER;
		byte lcdStatus = memory->readDirectly(LCDC_STATUS);
		setBit(&lcdStatus, 1);
		setBit(&lcdStatus, 0);
		memory->writeDirectly(LCDC_STATUS, lcdStatus);
	}
}

void Video::handleLCDTransferMode() {
	if (videoCycles >= MIN_LCD_TRANSFER_CYCLES) {
		videoCycles = 0;

		/* This can be optimize so it is done before we change the mode */
		drawScanline();

		/* Move to mode 0 */
		mode = Mode::H_BLANK;
		byte lcdStatus = memory->readDirectly(LCDC_STATUS);
		clearBit(&lcdStatus,1);
		clearBit(&lcdStatus,0);
		memory->writeDirectly(LCDC_STATUS, lcdStatus);
	}
}

void Video::compareLYWithLYC() {
	byte currentScanline = memory->readDirectly(LY_REGISTER);
	byte lyCompare = memory->readDirectly(LY_COMPARE);
	byte lcdStatus = memory->readDirectly(LCDC_STATUS);

	if (lyCompare == currentScanline) {
		setBit(&lcdStatus, 2);
		if (isBitSet(lcdStatus, 6)) {
			cpu->requestInterrupt(Interrupts::LCD);
		}
	} else {
		clearBit(&lcdStatus, 2);
	}
	memory->writeDirectly(LCDC_STATUS, lcdStatus);
}

void Video::drawScanline() {
    const byte lcdControl = memory->readDirectly(LCD_CONTROL);
	if (isBitSet(lcdControl, 0)) {
		renderBackground(lcdControl);
	}
	if ( isBitSet(lcdControl, 1) ) {
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
        signed char tileNum;
        
        if( unsig ) {
            tileNum = static_cast<byte>(memory->readDirectly(backgroundMemory+tileRow + tileCol));
        } else {
            tileNum = static_cast<signed char>(memory->readDirectly(backgroundMemory+tileRow + tileCol));
        }

        word tileLocation = tileData;
        if ( unsig ) {
            tileLocation += (tileNum * 16);
			assert(tileLocation < 0x8FFF);
        } else {
            tileLocation += ((tileNum + 0x80) *16);
			assert(tileLocation < 0x97FF);
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
		frameBuffer[scanline][pixel].red = colour.red;
		frameBuffer[scanline][pixel].green = colour.green;
		frameBuffer[scanline][pixel].blue = colour.blue;
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

				if (!colour.isEqual(getColour(white))) {
					frameBuffer[scanline][pixel].red = colour.red;
					frameBuffer[scanline][pixel].green = colour.green;
					frameBuffer[scanline][pixel].blue = colour.blue;
				}
 			}
		}
	}
}

Video::Mode Video::getLCDMode() const {
    byte lcdStatus = memory->readDirectly(LCDC_STATUS);
	return Video::Mode(lcdStatus & 0x3);
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

void Video::switchPallete() {
	if (pallete == CLASSIC_PALLETE) {
		pallete = GREY_PALLETE;
	} else if (pallete == GREY_PALLETE) {
		pallete = CLASSIC_PALLETE;
	}
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

