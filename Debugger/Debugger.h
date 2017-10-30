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

#include "CPUDebugger.h"
#include "MemoryDebugger.h"
#include "VideoDebugger.h"
#include "CartridgeDebugger.h"

enum class DebuggerMode : int {
	IDDLE,
	BREAKPOINT,
	V_SYNC,
	RUNNING
};

class Debugger {
public:
	Debugger();

	void startDebugger(const CPU& cpu, const Memory& memory, Video& video, const Cartridge& cartridge);
	void update(int cycles, const CPU& cpu, const Memory& memory, Video& video, const Cartridge& cartridge);
	void closeDebugger();

private:
	const ImVec4 BACKGROUND_COLOR = {0.8f, 0.8f, 0.8f, 1.0f};

	GLFWwindow* window;
	DebuggerMode mode;
	CPUDebugger cpuDebugger;
	MemoryDebugger memoryDebugger;
	VideoDebugger videoDebugger;
	CartridgeDebugger cartridgeDebugger;

	void composeView(int cycles, const CPU& cpu, const Memory& memory, Video& video, const Cartridge& cartridge);
	void handleBreakpointHit(int cycles, const CPU& cpu, const Memory& memory, Video &video, const Cartridge& cartridge);
	void render();
};