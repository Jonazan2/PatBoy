#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "Imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "CPU.h"
#include "Types.h"
#include "Utils.h"

class Debugger {
public:
	enum class Mode : int {
		IDDLE,
		BREAKPOINT,
		V_SYNC,
		RUNNING
	};

	Debugger() {
		clear_color = ImColor(114, 144, 154);
		mode = Mode::RUNNING;
	}

	int startDebugger(CPU& cpu, Memory& memory)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(800, 600, "PatBoy Debugger", NULL, NULL);
		glfwMakeContextCurrent(window);

		GLint GlewInitResult = glewInit();
		if (GLEW_OK != GlewInitResult)
		{
			printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
			exit(EXIT_FAILURE);
		}

		ImGui_ImplGlfwGL3_Init(window, true);

		composeView(cpu, memory);
		render();
	}

	void update(int cycles, CPU& cpu, Memory& memory) {

		for (word breakpoint : breakpoints) {
			if (cpu.getPC().value == breakpoint) {
				handleBreakpointHit(cpu, memory);
				return;
			}
		}
		// for every frame that we render in the emulator, we render a frame in the debugger
		if (cycles >= 70224) {
			if (mode == Mode::V_SYNC) {
				handleBreakpointHit(cpu, memory);
			}

			composeView(cpu, memory);

			// Rendering
			render();
		}
	}

	void closeDebugger() {
		ImGui_ImplGlfwGL3_Shutdown();
		glfwTerminate();
	}

private:
	GLFWwindow* window;
	ImVec4 clear_color;
	Mode mode;

	std::vector<word> breakpoints;

	void composeView(CPU& cpu, Memory& memory) {
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		/* debugger controlers */
		ImGui::Separator();
		if (ImGui::Button("Continue")) {
			mode = Mode::RUNNING;
		}
		ImGui::SameLine();

		if (ImGui::Button("Run until vsync")) {
			mode = Mode::V_SYNC;
		}
		ImGui::Separator();

		/* registers */
		ImGui::Text("Registers:");
		ImGui::Separator();
		ImGui::Columns(2, "registers");
		ImGui::Text("AF: 0x%04X", cpu.getPC().value);
		ImGui::Text("DE: 0x%04X", cpu.getSP().value);
		ImGui::Text("PC: 0x%04X", cpu.getPC().value);
		ImGui::NextColumn();
		ImGui::Text("BC: 0x%04X", cpu.getBC().value);
		ImGui::Text("HL: 0x%04X", cpu.getHL().value);
		ImGui::Text("SP: 0x%04X", cpu.getSP().value);
		ImGui::NextColumn();
		ImGui::Columns(1);
		ImGui::Separator();
		
		/* flags*/
		const Register AF = cpu.getAF();
		ImGui::Text("Flags:");
		ImGui::Separator();
		ImGui::Columns(4, "flags");
		ImGui::Text("Zero");
		ImGui::Text(isBitSet(AF.low, ZERO_FLAG) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("Add/Sub");
		ImGui::Text(isBitSet(AF.low, ADD_SUB_FLAG) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("Half carry");
		ImGui::Text(isBitSet(AF.low, HALF_CARRY_FLAG) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("Carry");
		ImGui::Text(isBitSet(AF.low, CARRY_FLAG) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Columns(1);
		ImGui::Separator();
		
		/* opcodes */
		ImGui::BeginChild("##header", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y));
		ImGui::Columns(3);
		ImGui::Text("Address"); ImGui::NextColumn();
		ImGui::Text("Opcode"); ImGui::NextColumn();
		ImGui::Text("Cycles"); ImGui::NextColumn();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::EndChild();

		ImGui::BeginChild("##scrollingregion", ImVec2(0, 200));
		ImGui::Columns(3);

		byte previousOpcode = 0x00;
		for (int i = 0; i < 0x7FFF; i++)
		{
			byte opcode = memory.readDirectly(i);
			char label[32];
			sprintf(label, "0x%04X", i);

			if (ImGui::Selectable(label)) {
				breakpoints.push_back(i);
			}
			ImGui::NextColumn();
			
			Opcode opcodeInfo;
			if (previousOpcode != SPECIAL_OPCODE) {
				opcodeInfo = gameBoyOpcodes[opcode];
				if (opcodeInfo.additionalBytes == 2) {
					ImGui::Text(opcodeInfo.name, memory.readWordDirectly(i + 1)); ImGui::NextColumn();
				} else if (opcodeInfo.additionalBytes == 1) {
					ImGui::Text(opcodeInfo.name, memory.readDirectly(i + 1)); ImGui::NextColumn();
				} else {
					ImGui::Text(opcodeInfo.name); ImGui::NextColumn();
				}
				ImGui::Text("%02d", opcodeInfo.timing); ImGui::NextColumn();
			} else {
				opcodeInfo = specialGameBoyOpcodes[opcode];
				if (opcodeInfo.additionalBytes != 0) {
					
					ImGui::Text(opcodeInfo.name); ImGui::NextColumn();
				} else {
					ImGui::Text(opcodeInfo.name); ImGui::NextColumn();
				}
				ImGui::Text("%02d", opcodeInfo.timing); ImGui::NextColumn();
			}

			i = i + opcodeInfo.additionalBytes;
			previousOpcode = opcode;
		}
		ImGui::Columns(1);
		ImGui::EndChild();
		ImGui::Separator();

		/* Interrupts */
		const byte ieRegister = memory.readDirectly(IE_REGISTER);
		ImGui::Text("Interrupts:");
		ImGui::Separator();
		ImGui::Text("Interrupt Enable (0xFFFF):");
		ImGui::Columns(5, "flags");
		ImGui::Text("V-Blank");
		ImGui::Text(isBitSet(ieRegister, VBLANK) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("LCD Status");
		ImGui::Text(isBitSet(ieRegister, LCD) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("Timer");
		ImGui::Text(isBitSet(ieRegister, TIMER) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("Serial");
		ImGui::Text(isBitSet(ieRegister, SERIAL) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("Joypad");
		ImGui::Text(isBitSet(ieRegister, JOYPAD) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Columns(1);
		ImGui::Separator();

		const byte ifRegister = memory.readDirectly(IF_REGISTER);
		ImGui::Text("Interrupt Flag (0xFF0F):");
		ImGui::Columns(5, "flags");
		ImGui::Text("V-Blank");
		ImGui::Text(isBitSet(ifRegister, VBLANK) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("LCD Status");
		ImGui::Text(isBitSet(ifRegister, LCD) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("Timer");
		ImGui::Text(isBitSet(ifRegister, TIMER) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("Serial");
		ImGui::Text(isBitSet(ifRegister, SERIAL) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Text("Joypad");
		ImGui::Text(isBitSet(ifRegister, JOYPAD) ? "1" : "0"); ImGui::NextColumn();
		ImGui::Columns(1);
	}

	void handleBreakpointHit(CPU& cpu, Memory& memory) {
		std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
		previous = std::chrono::high_resolution_clock::now();
		
		mode = Mode::IDDLE;
		while (mode == Mode::IDDLE){
			current = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> (current - previous);
			previous = current;

			composeView(cpu, memory);
			render();

			static float DELAY_TIME = 1000.0f / 60;
			if (elapsed.count() < DELAY_TIME) {
				int waitTime = (int)(DELAY_TIME - elapsed.count());
				std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
			}
		}
	}

	void render() {
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		glfwSwapBuffers(window);
	}
};