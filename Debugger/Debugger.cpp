#include "Debugger.h"

#include <algorithm>
#include <sstream>
#include <thread>
#include <tuple>

#include "Imgui/imgui_impl_glfw_gl3.h"

#include "Utils.h"

Debugger::Debugger() : mode (DebuggerMode::RUNNING) {}

void Debugger::startDebugger(const CPU& cpu, const Memory& memory, Video &video, const Cartridge& cartridge)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(1200, 800, "PatBoy Debugger", NULL, NULL);
	glfwMakeContextCurrent(window);

	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	ImGui_ImplGlfwGL3_Init(window, true);
	videoDebugger.setFrameBufferTexture(ImGui_ImplGlfwGL3_CreateTexture(video.getFrameBuffer()));

	// we update first just in case we have a breakpoint in the first instruction
	update(0, cpu, memory, video, cartridge);

	composeView(0, cpu, memory, video, cartridge);
	render();
}

void Debugger::update(int cycles, const CPU& cpu, const Memory& memory, Video& video, const Cartridge &cartridge) {
	memoryDebugger.updateWatcher(memory, mode);

	if (mode == DebuggerMode::BREAKPOINT || cpuDebugger.addresshasBreakpoint(cpu.getPC().value)) {
		handleBreakpointHit(cycles, cpu, memory, video, cartridge);
	} else if (cycles >= 70224) {
		if (mode == DebuggerMode::V_SYNC) {
			handleBreakpointHit(cycles, cpu, memory, video, cartridge);
		}

		// for every frame (70224 cycles) that we render in the emulator, we render a frame in the debugger
		composeView(cycles, cpu, memory, video, cartridge);
		render();
	}
}

void Debugger::closeDebugger() {
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}

void Debugger::composeView(int cycles, const CPU& cpu, const Memory& memory, Video& video, const Cartridge& cartridge) {
	glfwPollEvents();
	ImGui_ImplGlfwGL3_NewFrame();

	cpuDebugger.startView(cycles, cpu, memory, mode);
	videoDebugger.startView(memory, video);
	memoryDebugger.startView(memory, mode);
	cartridgeDebugger.startView(cartridge);
}

void Debugger::handleBreakpointHit(int cycles, const CPU& cpu, const Memory& memory, Video& video, const Cartridge &cartridge) {
	std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
	previous = std::chrono::high_resolution_clock::now();

	mode = DebuggerMode::IDDLE;
	while (mode == DebuggerMode::IDDLE) {
		current = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (current - previous);
		previous = current;

		composeView(cycles, cpu, memory, video, cartridge);
		render();

		static float DELAY_TIME = 1000.0f / 57.3f;
		if (elapsed.count() < DELAY_TIME) {
			std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(DELAY_TIME - elapsed.count()));
		}
	}
}

void Debugger::render() {
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, BACKGROUND_COLOR.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
	glfwSwapBuffers(window);
}