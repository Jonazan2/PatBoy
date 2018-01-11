#pragma once

#include "Types.h"

class Memory;
class Video;

class VideoDebugger {
public:
	VideoDebugger();
	~VideoDebugger() = default;

	void startView(const Memory& memory, Video& video);
	void setFrameBufferTexture(void* textureId) { frameBufferTexture = textureId; }
	void setTileBufferTexture(void* textureId) { tileBufferTexture = textureId; }
	void setBackgroundBufferTexture(void* textureId) { backgroundBufferTexture = textureId; }

	void* getTileBufferTexture() { return tileBuffer; }
	void* getBackgroundBufferTexture() { return backgroundBuffer; }

private:

	static const RGB PINK;
	static const int TILE_BUFFER_WIDTH = 128;
	static const int TILE_BUFFER_HEIGHT = 192;
	static const int BACKGROUND_BUFFER_WIDTH = 256;
	static const int BACKGROUND_BUFFER_HEIGHT = 256;

	void* frameBufferTexture;
	bool showFrameBufferWindow;

	void* tileBufferTexture;
	bool showTileBufferWindow;
	RGB tileBuffer[TILE_BUFFER_WIDTH * TILE_BUFFER_HEIGHT];

	void* backgroundBufferTexture;
	bool showBackgroundBufferWindow;
	RGB backgroundBuffer[BACKGROUND_BUFFER_WIDTH * BACKGROUND_BUFFER_HEIGHT];

	void updateBackground(const Memory &memory, const Video &video);
	void updateBackground(const Memory &memory, const Video &video, word tileAddress, byte row, byte column);
	void drawWindowPosition(const Memory &memory, const Video &video);
	void drawLine(byte x, byte y, byte length, bool vertical);

	void updateTiles(const Memory &memory, const Video &video);
	void updateTiles(const Memory &memory, const Video &video, word start, byte row, byte column);
	byte getColour(byte upperByte, byte lowerByte, byte position) const;
};