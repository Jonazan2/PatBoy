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
	void* frameBufferTexture;
	bool showFrameBufferWindow;
	void* tileBufferTexture;
	bool showTileBufferWindow;
	RGB tileBuffer[192][128];

	void* backgroundBufferTexture;
	bool showBackgroundBufferWindow;
	RGB backgroundBuffer[256][256];

	void updateBackground(const Memory &memory, const Video &video);
	void updateBackground(const Memory &memory, const Video &video, word tileAddress, byte row, byte column);
	void drawWindowPosition(const Memory &memory, const Video &video);
	void drawLine(byte x, byte y, byte length, bool vertical);

	void updateTiles(const Memory &memory, const Video &video);
	void updateTiles(const Memory &memory, const Video &video, word start, byte row, byte column);
	byte getColour(byte upperByte, byte lowerByte, byte position) const;
};