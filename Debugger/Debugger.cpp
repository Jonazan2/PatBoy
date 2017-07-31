#include "Debugger.h"

#include <algorithm>
#include <sstream>
#include <thread>
#include <tuple>

#include "Imgui/imgui_impl_glfw_gl3.h"

#include "Utils.h"

Debugger::Debugger() {
	mode = Mode::RUNNING;
	watcherAsBreakpoint = false;
}

void Debugger::startDebugger(const CPU& cpu, const Memory& memory, const Video &video, const Cartridge& cartridge)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

	// we update first just in case we have a breakpoint in the first instruction
	update(0, cpu, memory, video, cartridge);

	composeView(0, cpu, memory, video, cartridge);
	render();
}

void Debugger::update(int cycles, const CPU& cpu, const Memory& memory, const Video& video, const Cartridge &cartridge) {
	if (!watcher.empty()) {
		if (watcherAsBreakpoint && watcherDataHasChanged(memory)) {
			mode = Mode::BREAKPOINT;
		}
		updateWatcherData(memory);
	}

	if (mode == Mode::BREAKPOINT || addresshasBreakpoint(cpu.getPC().value)) {
		handleBreakpointHit(cycles, cpu, memory, video, cartridge);
	} else if (cycles >= 70224) {
		if (mode == Mode::V_SYNC) {
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

void Debugger::composeView(int cycles, const CPU& cpu, const Memory& memory, const Video& video, const Cartridge& cartridge) {
	glfwPollEvents();
	ImGui_ImplGlfwGL3_NewFrame();

	startCPUView(cycles, cpu, memory);
	startVideoView(cpu, memory, video);
	startMemoryView(memory);
	startCartridgeView(cartridge);
}

void Debugger::startCPUView(const int cycles, const CPU& cpu, const Memory& memory) {
	ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(650, 800), ImGuiSetCond_FirstUseEver);

	ImGui::Begin("CPU");
	/* debugger controlers */
	ImGui::Separator();
	if (ImGui::Button("Next instruction")) {
		mode = Mode::BREAKPOINT;
	}
	ImGui::SameLine();

	if (ImGui::Button("Run")) {
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
		ImGui::TextColored(ImVec4(255, 0, 0, 255), "PC: 0x%04X", cpu.getPC().value);
	}
	else {
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
	ImGuiListClipper clipper(0x10000, ImGui::GetTextLineHeightWithSpacing());
	while (clipper.Step()) {
		
		int end = clipper.DisplayEnd;
		if (clipper.DisplayEnd < 0xFFFF - 50) {
			end = clipper.DisplayEnd + 50;
		}
		else {
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
				}
				else {
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
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::Text("Breakpoints");
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.95f, 0.5f, 0.5f));
		if (ImGui::Button("Clear all breakpoints")) {
			breakpoints.clear();
		}
		ImGui::PopStyleColor(2);

		ImGui::Separator();
		ImGui::Columns(3, "breakpoints");

		std::set<word>::iterator it;
		for (it = breakpoints.begin(); it != breakpoints.end(); ) {
			char label[32];
			sprintf(label, "0x%04X", *it);

			if (ImGui::Selectable(label)) {
				it = breakpoints.erase(it);
			}
			else {
				++it;
			}
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
	}
	else {
		ImGui::Text("Breakpoints - Empty");
		ImGui::Separator();
	}

	/* Interrupts */
	const byte ieRegister = memory.readDirectly(IE_REGISTER);
	ImGui::Text("Interrupts:"); ImGui::SameLine();
	ImGui::Text("IME: %s", cpu.isIMEActive() ? "true" : "false");
	ImGui::Separator();
	ImGui::Text("Interrupt Enable (0xFFFF): 0x%02X", memory.readDirectly(0xFFFF));
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
	ImGui::Text("Interrupt Flag (0xFF0F): 0x%02X", memory.readDirectly(0xFF0F));
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

/* cycles counter */
ImGui::Separator();
ImGui::Text("Cycles executed: %d", cycles);

ImGui::End();
}

void Debugger::startVideoView(const CPU& cpu, const Memory& memory, const Video& video) {
	ImGui::SetNextWindowPos(ImVec2(300, 0), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(605, 80));

	ImGui::Begin("Video");
	ImGui::Text("LCD Control: 0x%02X", memory.readDirectly(Video::LCD_CONTROL));
	ImGui::Text("LCDC Status: 0x%02X", memory.readDirectly(Video::LCDC_STATUS));

	byte yCoordinate = memory.readDirectly(Video::LY_REGISTER);
	if (yCoordinate > 144) {
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "LY: %d", yCoordinate);
	}
	else {
		ImGui::Text("LY: %d", yCoordinate);
	}

	ImGui::End();
}

void Debugger::startMemoryView(const Memory& memory) {
	byte* mem_data = memory.getMap();

	ImGui::SetNextWindowPos(ImVec2(650, 150), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Memory");
	ImGui::BeginChild("##scrolling", ImVec2(0, 450));

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	int addr_digits_count = 0;
	for (int n = MEMORY_VIEW_BASE_ADDRESS + MEMORY_VIEW_MEMORY_SIZE - 1; n > 0; n >>= 4) {
		addr_digits_count++;
	}

	float glyph_width = ImGui::CalcTextSize("F").x;
	float cell_width = glyph_width * 3;

	float line_height = ImGui::GetTextLineHeight();
	int line_total_count = (int)((MEMORY_VIEW_MEMORY_SIZE + MEMORY_VIEW_ROWS - 1) / MEMORY_VIEW_ROWS);
	ImGuiListClipper clipper(line_total_count, line_height);
	int visible_start_addr = clipper.DisplayStart * MEMORY_VIEW_ROWS;
	int visible_end_addr = clipper.DisplayEnd * MEMORY_VIEW_ROWS;

	bool data_next = false;
	bool draw_separator = true;
	for (int line_i = clipper.DisplayStart; line_i < clipper.DisplayEnd; line_i++) {
		int addr = line_i * MEMORY_VIEW_ROWS;
		ImGui::Text("%0*X: ", addr_digits_count, MEMORY_VIEW_BASE_ADDRESS + addr);
		ImGui::SameLine();

		/* Draw Hexadecimal */
		float line_start_x = ImGui::GetCursorPosX();
		for (int n = 0; n < MEMORY_VIEW_ROWS && addr < MEMORY_VIEW_MEMORY_SIZE; n++, addr++) {
			ImGui::SameLine(line_start_x + cell_width * n);
			ImGui::Text("%02X ", mem_data[addr]);
		}

		ImGui::SameLine(line_start_x + cell_width * MEMORY_VIEW_ROWS + glyph_width * 2);

		if (draw_separator) {
			ImVec2 screen_pos = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddLine(ImVec2(screen_pos.x - glyph_width, screen_pos.y - 9999), ImVec2(screen_pos.x - glyph_width, screen_pos.y + 9999), ImColor(ImGui::GetStyle().Colors[ImGuiCol_Border]));
			draw_separator = false;
		}

		/* Draw ASCII values */
		addr = line_i * MEMORY_VIEW_ROWS;
		for (int n = 0; n < MEMORY_VIEW_ROWS && addr < MEMORY_VIEW_MEMORY_SIZE; n++, addr++) {
			if (n > 0) ImGui::SameLine();
			int c = mem_data[addr];
			ImGui::Text("%c", (c >= 32 && c < 128) ? c : '.');
		}
	}
	clipper.End();
	ImGui::PopStyleVar(2);
	ImGui::EndChild();

	/* Watcher */
	ImGui::Separator();

	ImGui::PushItemWidth(160);
	ImGui::AlignFirstTextHeightToWidgets();
	ImGui::Text("Add watcher:"); ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(70);
	char input[64];
	memset(input, 0, sizeof(char) * 64);
	if (ImGui::InputText("##addr", input, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
		int address;
		if (sscanf(input, "%X", &address)) {
			word validAddress = address & 0xFFFF;
			watcher.insert({ validAddress, mem_data[validAddress]});
		}
	}
	ImGui::PopItemWidth();

	if (!watcher.empty()) {
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.95f, 0.5f, 0.5f));
		if (ImGui::Button("Clear all watchers")) {
			watcher.clear();
			mode = Mode::RUNNING;
		}
		ImGui::PopStyleColor(2);
		ImGui::SameLine();

		ImGui::Checkbox("Data breakpoint", &watcherAsBreakpoint);

		ImGui::Separator();
		ImGui::Columns(3, "breakpoints");

		std::map<word, byte>::iterator it;
		for (it = watcher.begin(); it != watcher.end(); ) {
			word address = it->first;
			byte data = it->second;

			char label[64];
			sprintf(label, "0x%04X:  0x%02X  %c", address, data, (data >= 32 && data < 128) ? data : '.');
			if (ImGui::Selectable(label)) {
				it = watcher.erase(it);
			}
			else {
				++it;
			}
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
	}

	ImGui::End();
}

void Debugger::startCartridgeView(const Cartridge& cartridge) {
	ImGui::SetNextWindowPos(ImVec2(610, 0), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(475, 120));

	ImGui::Begin("Cartridge");

	ImGui::Columns(2, "cartridge");
	ImGui::Text("Title: %s", cartridge.getTitle().c_str());
	ImGui::Text("Region: %s", cartridge.getDestinationCodeName().c_str());
	ImGui::Text("CGB: %s", cartridge.getCGBFlagName().c_str());
	ImGui::Text("SGB: %s", cartridge.getSGBFlagName().c_str());
	ImGui::NextColumn();

	ImGui::Text("ROM size: %s", cartridge.getCartridgeSizeName().c_str());
	ImGui::Text("ROM banks: %d", cartridge.getAmountOfRomBanks());
	ImGui::Text("RAM size: %s", cartridge.getRamSizeName().c_str());
	ImGui::Text("RAM banks: %d", cartridge.getAmountOfRamBanks());
	ImGui::Columns(1);

	ImGui::Separator();
	ImGui::Text("Cartridge type: %s", cartridge.getCartridgeTypeName().c_str());

	ImGui::End();
}



void Debugger::handleBreakpointHit(int cycles, const CPU& cpu, const Memory& memory, const Video& video, const Cartridge &cartridge) {
	std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
	previous = std::chrono::high_resolution_clock::now();

	mode = Mode::IDDLE;
	while (mode == Mode::IDDLE) {
		current = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> (current - previous);
		previous = current;

		composeView(cycles, cpu, memory, video, cartridge);
		render();

		static float DELAY_TIME = 1000.0f / 60;
		if (elapsed.count() < DELAY_TIME) {
			int waitTime = (int)(DELAY_TIME - elapsed.count());
			std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
		}
	}
}

bool Debugger::addresshasBreakpoint(word address) const {
	std::set<word>::const_iterator it = breakpoints.find(address);
	return it != breakpoints.end();
}


bool Debugger::watcherDataHasChanged(const Memory &memory) const {
	std::map<word, byte>::const_iterator it;
	for (it = watcher.begin(); it != watcher.end(); ++it) {
		if (it->second != memory.readDirectly(it->first)) {
			return true;
		}
	}
	return false;
}

void Debugger::updateWatcherData(const Memory& memory) {
	std::map<word, byte>::iterator it;
	for (it = watcher.begin(); it != watcher.end(); ++it) {
		byte data = memory.readDirectly(it->first);
		if (it->second != data) {
			it->second = data;
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