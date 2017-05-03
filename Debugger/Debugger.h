#pragma once

#include <set>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "Imgui/imgui.h"

#include "CPU.h"
#include "Types.h"

class Debugger {
public:
	enum class Mode : int {
		IDDLE,
		BREAKPOINT,
		V_SYNC,
		RUNNING
	};

	Debugger();

	int startDebugger(CPU& cpu, Memory& memory);
	void update(int cycles, CPU& cpu, Memory& memory);
	void closeDebugger();

private:
	Mode mode;
	GLFWwindow* window;
	ImVec4 clear_color;
	std::set<word> breakpoints;

	void composeView(CPU& cpu, Memory& memory);
	void startCPUView(CPU& cpu, Memory& memory);

	bool addresshasBreakpoint(word address);
	void handleBreakpointHit(CPU& cpu, Memory& memory);
	void render();
};