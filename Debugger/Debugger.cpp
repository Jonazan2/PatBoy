#include "Debugger.h"

#include <algorithm>
#include <sstream>
#include <thread>
#include <tuple>

#include "Imgui/imgui_impl_glfw_gl3.h"

Debugger::Debugger(CPU* cpu, Memory *memory, Video *video, Cartridge *cartridge) 
	: mode (DebuggerMode::RUNNING) , cpu(cpu), memory(memory), video(video), cartridge(cartridge) {}

void Debugger::startDebugger()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(1200, 800, "PatBoy Debugger", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	ImGui_ImplGlfwGL3_Init(window, true);

	Texture frameTexture = {0, 160, 144, video->getFrameBuffer() };
	videoDebugger.setFrameBufferTexture(ImGui_ImplGlfwGL3_CreateTexture(frameTexture));

	Texture tileTexture = { 0, 128, 192, videoDebugger.getTileBufferTexture() };
	videoDebugger.setTileBufferTexture(ImGui_ImplGlfwGL3_CreateTexture(tileTexture));

	Texture backgroundTexture = { 0, 256, 256, videoDebugger.getBackgroundBufferTexture() };
	videoDebugger.setBackgroundBufferTexture(ImGui_ImplGlfwGL3_CreateTexture(backgroundTexture));

	// we update first just in case we have a breakpoint in the first instruction
	update(0);
	composeView(0);
	render();
}

void Debugger::update(unsigned int cycles) {
	memoryDebugger.updateWatcher(memory, mode);

	if (mode == DebuggerMode::BREAKPOINT || cpuDebugger.addresshasBreakpoint(cpu->getPC().value)) {
		handleBreakpointHit(cycles);
	} else if (cycles >= 70224) {
		if (mode == DebuggerMode::V_SYNC) {
			handleBreakpointHit(cycles);
		}

		// for every frame (70224 cycles) that we render in the emulator, we render a frame in the debugger
		composeView(cycles);
		render();
	}
}

void Debugger::closeDebugger() {
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}

void Debugger::composeView(unsigned int cycles) {
	glfwPollEvents();
	ImGui_ImplGlfwGL3_NewFrame();

	composeControlView();
	cpuDebugger.startView(cycles, cpu, memory, mode);
	videoDebugger.startView(memory, video);
	memoryDebugger.startView(memory, mode);
	cartridgeDebugger.startView(cartridge);
}

void Debugger::composeControlView() {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(200, 50));

	ImGui::Begin("Emulator Control");
	if (ImGui::Button("Reset")) {
		resetEmulator();
	}
	ImGui::End();
}

void Debugger::handleBreakpointHit(unsigned int cycles) {
	std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
	previous = std::chrono::high_resolution_clock::now();

	mode = DebuggerMode::IDLE;
	while (mode == DebuggerMode::IDLE) {
		current = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (current - previous);
		previous = current;

		composeView(cycles);
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
	ImGui_ImplGlfwGL3_RenderDrawLists(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

void Debugger::resetEmulator() {
	cpu->reset();
	video->reset();
}