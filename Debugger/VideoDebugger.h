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

	void* getTileBufferTexture() { return tileBuffer; }

private:
	void* frameBufferTexture;
	bool showFrameBufferWindow;
	void* tileBufferTexture;
	bool showTileBufferWindow;
	RGB tileBuffer[192][128];

	void updateTiles(const Memory& memory);
	void updateTiles(const Memory& memory, word start, byte row, byte column);
	byte getColour(byte upperByte, byte lowerByte, byte position);
};