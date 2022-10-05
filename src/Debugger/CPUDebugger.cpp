#include "CPUDebugger.h"

#include "Debugger.h"

CPUDebugger::CPUDebugger() : instructionJump(false) {}

void CPUDebugger::startView(int cycles, const CPU *cpu, const Memory *memory, DebuggerMode& mode) {
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(650, 800), ImGuiCond_FirstUseEver);

	ImGui::Begin("CPU");
	/* debugger controlers */
	ImGui::Separator();
	if (ImGui::Button("Next instruction")) {
		mode = DebuggerMode::BREAKPOINT;
		instructionJump = false;
	}
	ImGui::SameLine();

	if (ImGui::Button("Run")) {
		mode = DebuggerMode::RUNNING;
		instructionJump = false;
	}
	ImGui::SameLine();

	if (ImGui::Button("Run until vsync")) {
		mode = DebuggerMode::V_SYNC;
	}
	ImGui::Separator();

	/* registers */
	ImGui::Text("Registers:");
	ImGui::Separator();
	ImGui::Columns(2, "registers");
	ImGui::Text("AF: 0x%04X", cpu->getAF().value);
	ImGui::Text("DE: 0x%04X", cpu->getDE().value);
	if (mode == DebuggerMode::IDLE || mode == DebuggerMode::BREAKPOINT) {
		ImGui::TextColored(ImVec4(255, 0, 0, 255), "PC: 0x%04X", cpu->getPC().value);
	} else {
		ImGui::Text("PC: 0x%04X", cpu->getPC().value);
	}

	ImGui::NextColumn();
	ImGui::Text("BC: 0x%04X", cpu->getBC().value);
	ImGui::Text("HL: 0x%04X", cpu->getHL().value);
	ImGui::Text("SP: 0x%04X", cpu->getSP().value);
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();

	/* flags*/
	const Register AF = cpu->getAF();
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

	ImGui::BeginGroup();
	ImGui::BeginChild("##scrollingregion", ImVec2(0, 160));
	ImGuiListClipper clipper(0xFFFF, ImGui::GetTextLineHeightWithSpacing());

	if ((mode == DebuggerMode::IDLE || mode == DebuggerMode::BREAKPOINT) && !instructionJump) {
		ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + (cpu->getPC().value * ImGui::GetTextLineHeightWithSpacing()), 0.5f);
		instructionJump = true;
		clipper.DisplayEnd += 16;
	} else {
		clipper.DisplayEnd = clipper.DisplayStart + 30;
	}

	byte previousOpcode = 0x00;
	for (int i = clipper.DisplayStart; i <= clipper.DisplayEnd; i++) {

		byte opcode = memory->readDirectly(i);
		Opcode opcodeInfo;
		if (previousOpcode != SPECIAL_OPCODE) {
			opcodeInfo = gameBoyOpcodes[opcode];
		} else {
			opcodeInfo = specialGameBoyOpcodes[opcode];
		}

		char address[32];
		sprintf(address, "0x%04X", i);

		char opcodeHex[32];
		sprintf(opcodeHex, "0x%02X", opcode);

		char name[32];
		memset(name, 0, 32);
		if (opcodeInfo.additionalBytes == 2) {
			word add = memory->readWordDirectly(i + 1);
			sprintf(name, opcodeInfo.name, add);
		} else if (opcodeInfo.additionalBytes == 1) {
			sprintf(name, opcodeInfo.name, memory->readDirectly(i + 1));
		} else {
			sprintf(name, opcodeInfo.name);
		}

		char text[128];
		sprintf(text, "%-*s%-*s%-*s%d", perItemWidth, address, perItemWidth, name, perItemWidth, opcodeHex, opcodeInfo.timing);

		if ((mode == DebuggerMode::IDLE || mode == DebuggerMode::BREAKPOINT) && i == cpu->getPC().value) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 140, 0, 0));
		}

		bool alreadySelected = addresshasBreakpoint(i);
		if (ImGui::Selectable(text, alreadySelected, ImGuiSelectableFlags_AllowDoubleClick)) {
			if (alreadySelected) {
				breakpoints.erase(i);
			} else {
				breakpoints.insert(i);
			}
		}

		if ((mode == DebuggerMode::IDLE || mode == DebuggerMode::BREAKPOINT) && i == cpu->getPC().value) {
			ImGui::PopStyleColor();
		}

		i = i + opcodeInfo.additionalBytes;
		previousOpcode = opcode;
	}
	clipper.End();
	ImGui::EndChild();
	ImGui::EndGroup();

	ImGui::Separator();

	/* breakpoints */
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Add breakpoint:");
	ImGui::SameLine();

	ImGui::PushItemWidth(70);
	char input[64];
	memset(input, 0, sizeof(char) * 64);
	if (ImGui::InputText("##addr", input, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
		int address;
		if (sscanf(input, "%X", &address)) {
			word validAddress = address & 0xFFFF;
			breakpoints.insert(validAddress);
		}
	}
	ImGui::PopItemWidth();

	if (!breakpoints.empty()) {
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1.0f, 0.6f, 0.6f).Value);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.95f, 0.5f, 0.5f).Value);
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
			} else {
				++it;
			}
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
	}
	ImGui::Separator();

	/* Interrupts */
	const byte ieRegister = memory->readDirectly(IE_REGISTER);
	ImGui::Text("Interrupts:"); ImGui::SameLine();
	ImGui::Text("IME: %s", cpu->isIMEActive() ? "true" : "false");
	ImGui::Separator();
	ImGui::Text("Interrupt Enable (0xFFFF): 0x%02X", memory->readDirectly(0xFFFF));
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

	const byte ifRegister = memory->readDirectly(IF_REGISTER);
	ImGui::Text("Interrupt Flag (0xFF0F): 0x%02X", memory->readDirectly(0xFF0F));
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

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Cycles:");
	ImGui::SameLine();
	char buf[32];
	sprintf(buf, "%d/%d", cycles, 70224);
	float progress = (float)cycles / 70224.0f;
	ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);

	ImGui::End();
}

bool CPUDebugger::addresshasBreakpoint(word address) const {
	if (breakpoints.empty()) {
		return false;
	}

	for (word breakpoint : breakpoints) {
		if (breakpoint == address) {
			return true;
		} else if (breakpoint > address) {
			return false;
		}
	}
	return false;
}