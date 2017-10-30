#pragma once

class Memory;
class Video;

class VideoDebugger {
public:
	VideoDebugger();
	~VideoDebugger() = default;

	void startView(const Memory& memory, Video& video);
	void setFrameBufferTexture(void* textureId) { frameBufferTexture = textureId; }

private:
	void* frameBufferTexture;
	bool showFrameBufferWindow;
};