#include <stdio.h>
#include <set>
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

		update(0, cpu, memory);
	}

	void update(int cycles, CPU& cpu, Memory& memory) {

		if (addresshasBreakpoint(cpu.getPC().value) || mode == Mode::BREAKPOINT) {
			handleBreakpointHit(cpu, memory);
			return;
		}

		// for every frame that we render in the emulator, we render a frame in the debugger
		if (cycles >= 70224) {
			if (mode == Mode::V_SYNC) {
				handleBreakpointHit(cpu, memory);
			}

			composeView(cpu, memory);
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

	std::set<word> breakpoints;

	void composeView(CPU& cpu, Memory& memory) {
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();


		ImGui::SetNextWindowSize(ImVec2(300, 650), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("CPU");
		/* debugger controlers */
		ImGui::Separator();
		if (ImGui::Button("Next")) {
			mode = Mode::BREAKPOINT;
		}
		ImGui::SameLine();

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
		ImGui::Text("AF: 0x%04X", cpu.getAF().value);
		ImGui::Text("DE: 0x%04X", cpu.getDE().value);
		if (mode == Mode::IDDLE || mode == Mode::BREAKPOINT) {
			ImGui::TextColored(ImVec4(255, 0, 0, 255),"PC: 0x%04X", cpu.getPC().value);
		} else {
			ImGui::Text("PC: 0x%04X", cpu.getPC().value);
		}
		
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
		int perItemWidth = 25;
		ImGui::Text("%-*s%-*s%-*s%-*s", perItemWidth, "Address", perItemWidth, "Mnemonic", perItemWidth, "Opcode", perItemWidth, "Cycles");
		ImGui::Separator();

		ImGui::BeginChild("##scrollingregion", ImVec2(0, 160));
		ImGuiListClipper clipper(0x8000, ImGui::GetTextLineHeightWithSpacing());
		while (clipper.Step()) {
			int end, start;
			if (clipper.DisplayEnd < 0x7FFF - 50) {
				end = clipper.DisplayEnd + 50;
			} else {
				end = clipper.DisplayEnd;
			}

			byte previousOpcode = 0x00;
			for (int i = clipper.DisplayStart; i < end; i++)
			{
				byte opcode = memory.readDirectly(i);
				Opcode opcodeInfo;
				if (previousOpcode != SPECIAL_OPCODE) {
					opcodeInfo = gameBoyOpcodes[opcode];
				}
				else {
					opcodeInfo = specialGameBoyOpcodes[opcode];
				}

				char address[32];
				sprintf(address, "0x%04X", i);

				char opcodeHex[32];
				sprintf(opcodeHex, "0x%02X", opcode);

				char name[32];
				memset(name, 0, 32);
				if (opcodeInfo.additionalBytes == 2) {
					word add = memory.readWordDirectly(i + 1);
					sprintf(name, opcodeInfo.name, add);
				}
				else if (opcodeInfo.additionalBytes == 1) {
					sprintf(name, opcodeInfo.name, memory.readDirectly(i + 1));
				}
				else {
					sprintf(name, opcodeInfo.name);
				}

				char text[128];
				sprintf(text, "%-*s%-*s%-*s%d", perItemWidth, address, perItemWidth, name, perItemWidth, opcodeHex, opcodeInfo.timing);

				bool alreadySelected = addresshasBreakpoint(i);
				if (ImGui::Selectable(text, alreadySelected, ImGuiSelectableFlags_AllowDoubleClick)) {
					if (alreadySelected) {
						breakpoints.erase(i);
					} else {
						breakpoints.insert(i);
					}
				}

				i = i + opcodeInfo.additionalBytes;
				previousOpcode = opcode;
			}
		}
		ImGui::EndChild();
		ImGui::Separator();

		/* breakpoints */
		if (!breakpoints.empty()) {
			ImGui::Text("Breakpoints");
			ImGui::Separator();
			ImGui::Columns(3, "mycolumns3", false);  // 3-ways, no border

			std::set<word>::iterator it;
			for (it = breakpoints.begin(); it != breakpoints.end(); ) {
				char label[32];
				sprintf(label, "0x%04X", *it);

				if (ImGui::Selectable(label)) {
					it = breakpoints.erase(it);
				} else {
					++it;
				}
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
		} else {
			ImGui::Text("Breakpoints - Empty");
			ImGui::Separator();
		}

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

		ImGui::End();
	}

	bool addresshasBreakpoint(word address) {
		std::set<word>::const_iterator it = breakpoints.find(address);
		return it != breakpoints.end();
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