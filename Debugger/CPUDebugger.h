#pragma once

#include <set>
#include "Types.h"

class CPU;
class Memory;

enum class DebuggerMode;

class CPUDebugger {
public:
	CPUDebugger();
	~CPUDebugger() = default;

	void startView(int cycles, const CPU* cpu, const Memory* memory, DebuggerMode& mode);
	bool addresshasBreakpoint(word address) const;

private:
	bool instructionJump;
	std::set<word> breakpoints;
};