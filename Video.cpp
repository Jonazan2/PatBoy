#include "Video.h"
#include <map>

#define STB_IMAGE_IMPLEMENTATION 
#include "Debugger/Imgui/stb_image.h"

RGB Video::CLASSIC_PALLETE[4] = { { 155,188,15 }, { 139,172,15 }, { 48,98,48 }, { 15,56,15 } };
RGB Video::GREY_PALLETE[4] = { { 255,255,255 },{ 0xCC,0xCC,0xCC },{ 0x77,0x77,0x77 }, { 0x0,0x0,0x0 } };
const Video::WindowResolution Video::FULLSCREEN_WINDOW = { 1920, 1080 };
const Video::WindowResolution Video::WINDOW_RESOLUTIONS[MAX_AMOUNT_WINDOW_RESOLUTIONS] = 
{	
	{GAMEBOY_WIDTH, GAMEBOY_HEIGHT}, 
	{GAMEBOY_WIDTH * 5, GAMEBOY_HEIGHT * 5}, 
	FULLSCREEN_WINDOW 
};

Video::Video(Memory *memory, CPU *cpu) {
    this->memory = memory;
    this->cpu = cpu;
    this->createSDLWindow();
    this->resetFrameBuffer();
    this->renderGame();
    
	this->videoCycles = 0;
	this->vblankCycles = 0;
	this->mode = Video::Mode::OAM_RAM;
	this->currentPallete = GREY_PALLETE;
	this->currentResolutionIndex = 0;
	changeWindowSize();
}

void Video::reset()
{
	this->resetFrameBuffer();
	this->videoCycles = 0;
	this->vblankCycles = 0;
	this->mode = Video::Mode::OAM_RAM;
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
		vblankCycles = 0;
		memory->writeDirectly(LY_REGISTER, 0);

		byte lcdc = memory->readDirectly(LCDC_STATUS);
		memory->writeDirectly(LCDC_STATUS, lcdc & 0xFC);
		mode = Video::Mode::H_BLANK;

		compareLYWithLYC();
	}
}

void Video::handleHBlankMode() {
    if (videoCycles >= MIN_HBLANK_MODE_CYCLES) {
		videoCycles -= MIN_HBLANK_MODE_CYCLES;

		byte scanline = memory->readDirectly(LY_REGISTER);
		memory->writeDirectly(LY_REGISTER, ++scanline);
		compareLYWithLYC();
		if (scanline == VERTICAL_BLANK_SCAN_LINE) {
			mode = Mode::V_BLANK;

			/* Move to mode 1: VBLANK*/
			byte lcdStatus = memory->readDirectly(LCDC_STATUS);
			setBit(&lcdStatus, 0);
			memory->writeDirectly(LCDC_STATUS, lcdStatus);
			cpu->requestInterrupt(Interrupts::VBLANK);
		} else {
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
}

void Video::handleVBlankMode(short cycles) {
	vblankCycles += cycles;

	byte scanline = memory->readDirectly(LY_REGISTER);
    if (vblankCycles >= MAX_VIDEO_CYCLES) {
		vblankCycles = 0;

		if (scanline < VERTICAL_BLANK_SCAN_LINE_MAX) {
			memory->writeDirectly(LY_REGISTER, ++scanline);
		}
		compareLYWithLYC();
	}

	if (videoCycles >= VBLANK_CYCLES) {
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
		videoCycles -= MIN_OAM_MODE_CYCLES;

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
		videoCycles -= MIN_LCD_TRANSFER_CYCLES;

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
    byte scrollY = memory->read(SCROLL_Y);
    byte scrollX = memory->read(SCROLL_X);
    byte windowY = memory->read(WINDOWS_Y);
    byte windowX = memory->read(WINDOWS_X);

	if (windowX <= 0x07) {
		windowX = 0x00;
	} else {
		windowX -= 7;
	}

	const int scanline = memory->readDirectly(LY_REGISTER);
	bool usingWindow = isBitSet(lcdControl, 5) && windowY <= scanline;

	bool unsig = true;    
	word tileData;
    if ( isBitSet(lcdControl,4) ) {
        tileData = 0x8000;
    } else {
        tileData = 0x8800;
        unsig= false;
    }

	byte backgroundPallete = memory->read(0xFF47);
	word tileRow, tileLocation, tileMap;
	byte xPos = 0, yPos = 0;
    for ( int pixel = 0 ; pixel < 160; pixel++ ) {
		
		if (usingWindow && pixel >= windowX && scanline >= windowY) {
			xPos = pixel - windowX;
			yPos = scanline - windowY;

			if (isBitSet(lcdControl, 6)) {
				tileMap = 0x9C00;
			} else {
				tileMap = 0x9800;
			}
		} else {
			xPos = pixel + scrollX;
			yPos = scrollY + scanline;

			if (isBitSet(lcdControl, 3)) {
				tileMap = 0x9C00;
			} else {
				tileMap = 0x9800;
			}
		}
		
		tileRow = ((yPos / 8) * 32);
		tileLocation = tileData;
        int tileNum;
        if( unsig ) {
            tileNum = static_cast<byte>(memory->read(tileMap + tileRow + (xPos / 8)));
			tileLocation += (tileNum * 16);
			assert(tileLocation < 0x8FFF);
        } else {
            tileNum = static_cast<signed char>(memory->read(tileMap + tileRow + (xPos / 8)));
			tileLocation += ((tileNum + 0x80) * 16);
			assert(tileLocation < 0x97FF);
        }

        byte address = (yPos % 8) * 2;
        byte upperByte = memory->read(tileLocation + address);
        byte lowerByte = memory->read(tileLocation + address + 1);

        int colourBit = xPos % 8;
        colourBit -= 7;
        colourBit = -colourBit;

        int colourNum = getBitValue(lowerByte, colourBit);
        colourNum <<= 1;
        colourNum |= getBitValue(upperByte,colourBit);

        RGB colour = currentPallete[getColourFromPallete(backgroundPallete, Colour(colourNum))];
		int index = pixel + (scanline * GAMEBOY_WIDTH);
		frameBuffer[index].red = colour.red;
		frameBuffer[index].green = colour.green;
		frameBuffer[index].blue = colour.blue;
    }
}

void Video::renderSprites(byte lcdControl) {
	bool use8x16 = isBitSet(lcdControl, 2);

    for (int sprite = 0 ; sprite < 40; sprite++) {
        byte index = sprite*4 ;

		// Read the sprite position and the its tile index
        byte yPos = memory->read(0xFE00+index) - 16;
        byte xPos = memory->read(0xFE00+index+1)-8;
        byte tileIndex = memory->read(0xFE00+index+2) ;

		// Read the sprite attributes
		byte attributes = memory->read(0xFE00+index+3) ;
		byte pallete = memory->read(isBitSet(attributes, 4) ? 0xFF49 : 0xFF48);
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
            byte upperByte = memory->read((0x8000 + (tileIndex * 0x10)) + line);
            byte lowerByte = memory->read((0x8000 + (tileIndex * 0x10)) + line+1);

            for ( int tilePixel = 7; tilePixel >= 0; tilePixel-- ) {
                
				int xPix = 0 - tilePixel;
				xPix += 7;

				int pixel = xPos + xPix;
				if (pixel < 160 && scanline < 144) {

					int position = tilePixel;
					if (xFlip) {
						position -= 7;
						position *= -1;
					}

					byte colourNum = 0x00;
					if (getBitValue(lowerByte, position)) {
						setBit(&colourNum, 1);
					}
					if (getBitValue(upperByte, position)) {
						setBit(&colourNum, 0);
					}

					if (colourNum != Colour::WHITE) {
						int index = pixel + (scanline * GAMEBOY_WIDTH);

						bool showThroughBG = hidden && frameBuffer[index].isEqual(currentPallete[Colour::WHITE]);
						if (!hidden || showThroughBG) {
							RGB colour = currentPallete[getColourFromPallete(pallete, Colour(colourNum))];
							frameBuffer[index].red = colour.red;
							frameBuffer[index].green = colour.green;
							frameBuffer[index].blue = colour.blue;
						}
					}
				}
 			}
		}
	}
}

Video::Mode Video::getLCDMode() const {
    byte lcdStatus = memory->readDirectly(LCDC_STATUS);
	return Video::Mode(lcdStatus & 0x3);
}

Video::Colour Video::getColourFromPallete(byte pallete, Colour originalColour) {
	byte colourNumber = 0;
	switch (originalColour) {
		case Colour::WHITE:
		if (getBitValue(pallete, 1)) {
			setBit(&colourNumber, 1);
		}
		if (getBitValue(pallete, 0)) {
			setBit(&colourNumber, 0);
		}
		break;
		case Colour::LIGHT_GREY:
		if (getBitValue(pallete, 3)) {
			setBit(&colourNumber, 1);
		}
		if (getBitValue(pallete, 2)) {
			setBit(&colourNumber, 0);
		}
		break;
		case Colour::DARK_GREY:
		if (getBitValue(pallete, 5)) {
			setBit(&colourNumber, 1);
		}
		if (getBitValue(pallete, 4)) {
			setBit(&colourNumber, 0);
		}
		break;
		case Colour::BLACK:
		if (getBitValue(pallete, 7)) {
			setBit(&colourNumber, 1);
		}
		if (getBitValue(pallete, 6)) {
			setBit(&colourNumber, 0);
		}
		break;
	}

	return Colour(colourNumber);
}


bool Video::createSDLWindow() {
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
        return false;
    }
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

	const WindowResolution resolution = WINDOW_RESOLUTIONS[currentResolutionIndex];
	window = SDL_CreateWindow("PatBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution.width, resolution.height, SDL_WINDOW_INPUT_FOCUS);

	setWindowIcon();
    renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, GAMEBOY_WIDTH, GAMEBOY_HEIGHT);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                SDL_TEXTUREACCESS_STREAMING, GAMEBOY_WIDTH, GAMEBOY_HEIGHT);
    return true ;
}

bool Video::setWindowIcon() {
	int req_format = STBI_rgb_alpha;
	int width, height, orig_format;
	icon = stbi_load("Resources/icon.png", &width, &height, &orig_format, req_format);
	if (icon == nullptr) {
		return false;
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)icon, 16, 16, 32, 4 * width,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_SetWindowIcon(window, surface);
}

void Video::renderGame() {
	assert(renderer && texture);
    SDL_UpdateTexture(texture, NULL, this->frameBuffer, GAMEBOY_WIDTH * sizeof(byte) * 3);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Video::switchPallete() {
	if (currentPallete == CLASSIC_PALLETE) {
		currentPallete = GREY_PALLETE;
	} else if (currentPallete == GREY_PALLETE) {
		currentPallete = CLASSIC_PALLETE;
	}
}

void Video::changeWindowSize()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	currentResolutionIndex = (currentResolutionIndex + 1) % MAX_AMOUNT_WINDOW_RESOLUTIONS;
	const WindowResolution resolution = WINDOW_RESOLUTIONS[currentResolutionIndex];
	window = SDL_CreateWindow("PatBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution.width, resolution.height, SDL_WINDOW_INPUT_FOCUS );
	if (resolution == FULLSCREEN_WINDOW) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	setWindowIcon();
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, GAMEBOY_WIDTH, GAMEBOY_HEIGHT);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
		SDL_TEXTUREACCESS_STREAMING, GAMEBOY_WIDTH, GAMEBOY_HEIGHT);
}

void Video::resetFrameBuffer() {
	memset(frameBuffer, 0xFF, sizeof(frameBuffer));
}

Video::~Video() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	stbi_image_free(icon);
    SDL_Quit();
}
