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
	IDLE,
	BREAKPOINT,
	V_SYNC,
	RUNNING
};

class Debugger {
public:
	Debugger(CPU* cpu, Memory *memory, Video *video, Cartridge *cartridge);

	void startDebugger();
	void update(unsigned int cycles);
	void closeDebugger();

private:
	const ImVec4 BACKGROUND_COLOR = {0.8f, 0.8f, 0.8f, 1.0f};

	CPU* cpu;
	Memory *memory;
	Video *video;
	Cartridge *cartridge;

	GLFWwindow* window;
	DebuggerMode mode;
	CPUDebugger cpuDebugger;
	MemoryDebugger memoryDebugger;
	VideoDebugger videoDebugger;
	CartridgeDebugger cartridgeDebugger;

	void composeView(unsigned int cycles);
	void handleBreakpointHit(unsigned int cycles);
	void render();
};