#include "VideoDebugger.h"

#include "Debugger.h"

VideoDebugger::VideoDebugger() : frameBufferTexture(nullptr), showFrameBufferWindow(false) ,
	showTileBufferWindow(false) {

	memset(tileBuffer, 0xFF, sizeof(tileBuffer));
};

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

	if (ImGui::Button(showTileBufferWindow ? "Hide tiles" : "Show tiles")) {
		showTileBufferWindow = !showTileBufferWindow;
	}

	if (showTileBufferWindow) {
		updateTiles(memory);
		ImGui::SetNextWindowSize(ImVec2(256, 384), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Tiles");
		ImGui::Image(tileBufferTexture, ImVec2(256, 384));
		ImGui::End();
	}

	ImGui::End();
}

void VideoDebugger::updateTiles(const Memory& memory) {
	word tileData = 0x8000;
	for (int row = 0; row < 24; row++) {
		for (int column = 0; column < 16; column++) {
			word start = tileData + (16 * column);
			updateTiles(memory, start, row, column);
		}
		tileData += 0x100;
	}
}

void VideoDebugger::updateTiles(const Memory& memory, word start, byte row, byte column) {
	static RGB GREY_PALLETE[4] = { { 255,255,255 },{ 0xCC,0xCC,0xCC },{ 0x77,0x77,0x77 },{ 0x0,0x0,0x0 } };

	for (int line = (row*8); line < 8 + (row * 8); line++, start+=2) {
		byte upperByte = memory.read(start);
		byte lowerByte = memory.read(start + 1);

		for (int tileColumn = (column*8), position = 7; tileColumn < 8 + (column * 8); tileColumn++, position--) {
			tileBuffer[line][tileColumn] = GREY_PALLETE[getColour(upperByte, lowerByte, position)];
		}
	}
}

byte VideoDebugger::getColour(byte upperByte, byte lowerByte, byte position) {
	byte colour = 0;
	if (isBitSet(upperByte, position))
		setBit(&colour, 1);
	if (isBitSet(lowerByte, position))
		setBit(&colour, 0);
	return colour;
}
