#include "VideoDebugger.h"

#include "Debugger.h"

const RGB VideoDebugger::PINK = { 0xFF, 0x14, 0x93 };

VideoDebugger::VideoDebugger() : frameBufferTexture(nullptr), showFrameBufferWindow(false) ,
	showTileBufferWindow(false), showBackgroundBufferWindow(false) {

	memset(tileBuffer, 0xFF, sizeof(tileBuffer));
	memset(backgroundBuffer, 0xFF, sizeof(backgroundBuffer));
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

	if (ImGui::Button(showTileBufferWindow ? "Hide vram" : "Show vram")) {
		showTileBufferWindow = !showTileBufferWindow;
	}

	if (showTileBufferWindow) {
		updateTiles(memory, video);
		ImGui::SetNextWindowSize(ImVec2(256, 384), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Tiles");
		ImGui::Image(tileBufferTexture, ImVec2(256, 384));
		ImGui::End();
	}
	ImGui::SameLine();

	if (ImGui::Button(showBackgroundBufferWindow ? "Hide background" : "Show background")) {
		showBackgroundBufferWindow = !showBackgroundBufferWindow;
	}

	if (showBackgroundBufferWindow) {
		updateBackground(memory, video);
		ImGui::SetNextWindowSize(ImVec2(512, 512), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Background");
		ImGui::Image(backgroundBufferTexture, ImVec2(512, 512));
		ImGui::End();
	}

	ImGui::End();
}

void VideoDebugger::updateBackground(const Memory& memory, const Video &video)
{
	byte lcdc = memory.readDirectly(0xFF40);

	bool unsignedIndex;
	word tileData;
	if (isBitSet(lcdc, 4)) {
		tileData = 0x8000;
		unsignedIndex = true;
	} else {
		tileData = 0x8800;
		unsignedIndex = false;
	}

	word tileMap;
	if (isBitSet(lcdc, 3)) {
		tileMap = 0x9C00;
	} else {
		tileMap = 0x9800;
	}

	for (byte row = 0; row < 32; row++) {
		for (byte column = 0; column < 32; column++) {
			if (unsignedIndex) {
				word index = memory.read(tileMap + row + column);
				updateBackground(memory, video, tileData + (index * 0x80), row, column);
			} else {
				signed char index = static_cast<signed char>(memory.read(tileMap + row + column));
				word location = ((index + 0x80) * 0x10);
				updateBackground(memory, video, tileData + location, row, column);
			}
		}
		tileMap += 0x1F;
	}

	drawWindowPosition(memory, video);
}

void VideoDebugger::updateBackground(const Memory& memory, const Video &video, word tileAddress, byte row, byte column) {
	for (int line = (row * 8); line < 8 + (row * 8); line++, tileAddress += 2) {
		byte upperByte = memory.read(tileAddress);
		byte lowerByte = memory.read(tileAddress + 1);

		for (int tileColumn = (column * 8), position = 7; tileColumn < 8 + (column * 8); tileColumn++, position--) {
			backgroundBuffer[line][tileColumn] = video.getCurrentPallete()[getColour(upperByte, lowerByte, position)];
		}
	}
}

void VideoDebugger::drawWindowPosition(const Memory& memory, const Video &video) {
	byte x = memory.read(0xFF43);
	byte y = memory.read(0xFF42);

	drawLine(x, y, 160, false);
	drawLine(x, y + 144, 160, false);
	drawLine(x, y, 144, true);
	drawLine(x + 160, y, 144, true);
}

void VideoDebugger::drawLine(byte x, byte y, byte length, bool vertical) {
	for (byte pixel = 0; pixel < length; pixel++) {
		backgroundBuffer[y][x] = PINK;
		vertical ? y++ : x++;
	}
}


void VideoDebugger::updateTiles(const Memory& memory, const Video &video) {
	word tileData = 0x8000;
	for (int row = 0; row < 24; row++) {
		for (int column = 0; column < 16; column++) {
			word start = tileData + (16 * column);
			updateTiles(memory, video, start, row, column);
		}
		tileData += 0x100;
	}
}

void VideoDebugger::updateTiles(const Memory& memory, const Video &video, word start, byte row, byte column) {
	for (int line = (row*8); line < 8 + (row * 8); line++, start+=2) {
		byte upperByte = memory.read(start);
		byte lowerByte = memory.read(start + 1);

		for (int tileColumn = (column*8), position = 7; tileColumn < 8 + (column * 8); tileColumn++, position--) {
			tileBuffer[line][tileColumn] = video.getCurrentPallete()[getColour(upperByte, lowerByte, position)];
		}
	}
}

byte VideoDebugger::getColour(byte upperByte, byte lowerByte, byte position) const {
	byte colour = 0;
	if (isBitSet(upperByte, position))
		setBit(&colour, 1);
	if (isBitSet(lowerByte, position))
		setBit(&colour, 0);
	return colour;
}