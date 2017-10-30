#include "VideoDebugger.h"

#include "Debugger.h"

VideoDebugger::VideoDebugger() : frameBufferTexture(nullptr), showFrameBufferWindow(false) {};

void VideoDebugger::startView(const Memory& memory, Video& video) {
	ImGui::SetNextWindowPos(ImVec2(300, 0), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(605, 80));

	ImGui::Begin("Video");
	ImGui::Columns(2);
	ImGui::Text("LCD Control: 0x%02X", memory.readDirectly(Video::LCD_CONTROL));
	ImGui::Text("LCDC Status: 0x%02X", memory.readDirectly(Video::LCDC_STATUS));

	byte yCoordinate = memory.readDirectly(Video::LY_REGISTER);
	if (yCoordinate > 144) {
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "LY: %d", yCoordinate);
	} else {
		ImGui::Text("LY: %d", yCoordinate);
	}

	ImGui::NextColumn();
	if (ImGui::Button(showFrameBufferWindow ? "Hide frame buffer" : "Show frame buffer")) {
		showFrameBufferWindow = !showFrameBufferWindow;
	}

	if (showFrameBufferWindow) {
		ImGui::SetNextWindowSize(ImVec2(160, 144), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("GameBoy");
		ImGui::Image(frameBufferTexture, ImVec2(160, 144));
		ImGui::End();
	}

	ImGui::End();
}
