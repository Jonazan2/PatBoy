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

	int startDebugger(const CPU& cpu, const Memory& memory, const Video& video);
	void update(int cycles, const CPU& cpu, const Memory& memory, const Video& video);
	void closeDebugger();

private:
	enum class Mode : int {
		IDDLE,
		BREAKPOINT,
		V_SYNC,
		RUNNING
	};

	Mode mode;
	GLFWwindow* window;
	ImVec4 clear_color;
	std::set<word> breakpoints;

	void composeView(int cycles, const CPU& cpu, const Memory& memory, const Video& video);
	void startCPUView(int cycles, const CPU& cpu, const Memory& memory);
	void startVideoView(const CPU& cpu, const Memory& memory, const Video& video);

	bool addresshasBreakpoint(word address);
	void handleBreakpointHit(int cycles, const CPU& cpu, const Memory& memory, const Video &video);
	
	void render();
};