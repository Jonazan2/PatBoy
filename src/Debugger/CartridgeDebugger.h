#pragma once

class Cartridge;

class CartridgeDebugger {
public:
	CartridgeDebugger() = default;
	~CartridgeDebugger() = default;

	void startView(const Cartridge *cartridge);
};