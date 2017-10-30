#include "MemoryDebugger.h"

#include "Debugger.h"

MemoryDebugger::MemoryDebugger() : watcherAsBreakpoint(false) {}

void MemoryDebugger::startView(const Memory& memory, DebuggerMode& mode) {

	ImGui::SetNextWindowPos(ImVec2(650, 150), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Memory");
	ImGui::BeginChild("##scrolling", ImVec2(0, 450));

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	int addressDigits = 0;
	for (int n = MEMORY_VIEW_BASE_ADDRESS + MEMORY_VIEW_MEMORY_SIZE - 1; n > 0; n >>= 4) {
		addressDigits++;
	}

	float glyphWidth = ImGui::CalcTextSize("F").x;
	float cellWidth = glyphWidth * 3;

	float height = ImGui::GetTextLineHeight();
	int totalLines = (int)((MEMORY_VIEW_MEMORY_SIZE + MEMORY_VIEW_ROWS - 1) / MEMORY_VIEW_ROWS);
	ImGuiListClipper clipper(totalLines, height);
	int visibleStartAddr = clipper.DisplayStart * MEMORY_VIEW_ROWS;
	int visibleEndAddr = clipper.DisplayEnd * MEMORY_VIEW_ROWS;

	bool drawSeparator = true;
	for (int lineNumber = clipper.DisplayStart; lineNumber < clipper.DisplayEnd; lineNumber++) {
		int addr = lineNumber * MEMORY_VIEW_ROWS;
		ImGui::Text("%0*X: ", addressDigits, MEMORY_VIEW_BASE_ADDRESS + addr);
		ImGui::SameLine();

		/* Draw Hexadecimal */
		float lineStartX = ImGui::GetCursorPosX();
		for (int n = 0; n < MEMORY_VIEW_ROWS && addr < MEMORY_VIEW_MEMORY_SIZE; n++, addr++) {
			ImGui::SameLine(lineStartX + cellWidth * n);
			ImGui::Text("%02X ", memory.getMap()[addr]);
		}

		ImGui::SameLine(lineStartX + cellWidth * MEMORY_VIEW_ROWS + glyphWidth * 2);

		if (drawSeparator) {
			ImVec2 screenPosition = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddLine(ImVec2(screenPosition.x - glyphWidth, screenPosition.y - 9999), ImVec2(screenPosition.x - glyphWidth, screenPosition.y + 9999), ImColor(ImGui::GetStyle().Colors[ImGuiCol_Border]));
			drawSeparator = false;
		}

		/* Draw ASCII values */
		addr = lineNumber * MEMORY_VIEW_ROWS;
		for (int n = 0; n < MEMORY_VIEW_ROWS && addr < MEMORY_VIEW_MEMORY_SIZE; n++, addr++) {
			if (n > 0) ImGui::SameLine();
			int c = memory.getMap()[addr];
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
			watcher.insert({ validAddress, memory.getMap()[validAddress] });
		}
	}
	ImGui::PopItemWidth();

	if (!watcher.empty()) {
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.95f, 0.5f, 0.5f));
		if (ImGui::Button("Clear all watchers")) {
			watcher.clear();
			mode = DebuggerMode::RUNNING;
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
			} else {
				++it;
			}
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
	}

	ImGui::End();
}

void MemoryDebugger::updateWatcher(const Memory& memory, DebuggerMode& mode) {
	if (!watcher.empty()) {
		if (watcherAsBreakpoint && watcherDataHasChanged(memory)) {
			mode = DebuggerMode::BREAKPOINT;
		}
		updateWatcherData(memory);
	}
}

bool MemoryDebugger::watcherDataHasChanged(const Memory& memory) const {
	std::map<word, byte>::const_iterator it;
	for (it = watcher.begin(); it != watcher.end(); ++it) {
		if (it->second != memory.readDirectly(it->first)) {
			return true;
		}
	}
	return false;
}

void MemoryDebugger::updateWatcherData(const Memory& memory) {
	std::map<word, byte>::iterator it;
	for (it = watcher.begin(); it != watcher.end(); ++it) {
		byte data = memory.readDirectly(it->first);
		if (it->second != data) {
			it->second = data;
		}
	}
}
