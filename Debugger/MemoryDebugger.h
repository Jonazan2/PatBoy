#pragma once

#include <map>
#include "Types.h"

class Memory;

enum class DebuggerMode;

class MemoryDebugger {
public:
	MemoryDebugger();
	~MemoryDebugger() = default;

	void startView(const Memory& memory, DebuggerMode& mode);
	void updateWatcher(const Memory& memory, DebuggerMode& mode);

private:
	const int MEMORY_VIEW_ROWS = 16;
	const int MEMORY_VIEW_MEMORY_SIZE = 0x10000;
	const int MEMORY_VIEW_BASE_ADDRESS = 0x0000;

	std::map<word, byte> watcher;
	bool watcherAsBreakpoint;

	bool watcherDataHasChanged(const Memory& memory) const;
	void updateWatcherData(const Memory& memory);
};