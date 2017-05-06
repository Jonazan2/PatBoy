#pragma once

#include <set>

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

	void startDebugger(const CPU& cpu, const Memory& memory, const Video& video);
	void update(int cycles, const CPU& cpu, const Memory& memory, const Video& video);
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

	void composeView(int cycles, const CPU& cpu, const Memory& memory, const Video& video);
	void startCPUView(int cycles, const CPU& cpu, const Memory& memory);
	void startVideoView(const CPU& cpu, const Memory& memory, const Video& video);
	void startMemoryView(const Memory& memory);

	bool addresshasBreakpoint(word address);
	void handleBreakpointHit(int cycles, const CPU& cpu, const Memory& memory, const Video &video);

	void render();
};