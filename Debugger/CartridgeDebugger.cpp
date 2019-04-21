#include "CartridgeDebugger.h"

#include "Debugger.h"

void CartridgeDebugger::startView(const Cartridge *cartridge) {
	ImGui::SetNextWindowPos(ImVec2(610, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(475, 120));

	ImGui::Begin("Cartridge");
	ImGui::Separator();

	ImGui::Columns(2, "cartridge");
	ImGui::Text("Title: %s", cartridge->getTitle().c_str());
	ImGui::Text("Region: %s", cartridge->getDestinationCodeName().c_str());
	ImGui::Text("CGB: %s", cartridge->getCGBFlagName().c_str());
	ImGui::Text("SGB: %s", cartridge->getSGBFlagName().c_str());
	ImGui::NextColumn();

	ImGui::Text("ROM size: %s", cartridge->getCartridgeSizeName().c_str());
	ImGui::Text("ROM banks: %d", cartridge->getAmountOfRomBanks());
	ImGui::Text("RAM size: %s", cartridge->getRamSizeName().c_str());
	ImGui::Text("RAM banks: %d", cartridge->getAmountOfRamBanks());
	ImGui::Columns(1);

	ImGui::Separator();
	ImGui::Text("Cartridge type: %s", cartridge->getCartridgeTypeName().c_str());

	ImGui::End();
}
