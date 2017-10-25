#pragma once

#include <set>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "Imgui/imgui.h"

#include "CPU.h"
#include "Video.h"
#include "Types.h"

class Debugger {
public:
	Debugger();

	void startDebugger(const CPU& cpu, const Memory& memory, Video& video, const Cartridge& cartridge);
	void update(int cycles, const CPU& cpu, const Memory& memory, Video& video, const Cartridge& cartridge);
	void closeDebugger();

private:

	const ImVec4 BACKGROUND_COLOR = {0.8f, 0.8f, 0.8f, 1.0f};

	const int MEMORY_VIEW_ROWS = 16;
	const int MEMORY_VIEW_MEMORY_SIZE = 0x10000;
	const int MEMORY_VIEW_BASE_ADDRESS = 0x0000;

	enum class Mode : int {
		IDDLE,
		BREAKPOINT,
		V_SYNC,
		RUNNING
	};

	Mode mode;
	GLFWwindow* window;
	std::set<word> breakpoints;
	std::map<word, byte> watcher;
	bool watcherAsBreakpoint;
	bool instructionJump;
	ImTextureID frameBufferTexture;
	bool showFrameBufferWindow;


	void composeView(int cycles, const CPU& cpu, const Memory& memory, Video& video, const Cartridge& cartridge);
	void startCPUView(int cycles, const CPU& cpu, const Memory& memory);
	void startVideoView(const CPU& cpu, const Memory& memory, Video& video);
	void startMemoryView(const Memory& memory);
	void startCartridgeView(const Cartridge& cartridge);

	bool addresshasBreakpoint(word address) const;
	bool watcherDataHasChanged(const Memory& memory) const;
	void updateWatcherData(const Memory& memory);
	void handleBreakpointHit(int cycles, const CPU& cpu, const Memory& memory, Video &video, const Cartridge& cartridge);

	void render();
};