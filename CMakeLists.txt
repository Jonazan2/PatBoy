cmake_minimum_required(VERSION 3.10)
project(PatBoy)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(.)
include_directories(Audio)
include_directories(Cpu)
include_directories(Debugger)
include_directories(Debugger/Imgui)
include_directories(Memory)

add_executable(PatBoy
        Audio/blargg_common.h
        Audio/blargg_config.h
        Audio/blargg_source.h
        Audio/Blip_Buffer.cpp
        Audio/Blip_Buffer.h
        Audio/Blip_Synth.h
        Audio/Effects_Buffer.cpp
        Audio/Effects_Buffer.h
        Audio/Gb_Apu.cpp
        Audio/Gb_Apu.h
        Audio/Gb_Apu_State.cpp
        Audio/Gb_Oscs.cpp
        Audio/Gb_Oscs.h
        Audio/Multi_Buffer.cpp
        Audio/Multi_Buffer.h
        Audio/Sound_Queue.cpp
        Audio/Sound_Queue.h
        Cpu/CPU_ALU.cpp
        Cpu/CPU_Control.cpp
        Cpu/CPU_Jumps.cpp
        Cpu/CPU_Loads.cpp
        Cpu/CPU_Rotate.cpp
        Cpu/CPU_SingleBit.cpp
        Debugger/Imgui/imconfig.h
        Debugger/Imgui/imgui.cpp
        Debugger/Imgui/imgui.h
        Debugger/Imgui/imgui_draw.cpp
        Debugger/Imgui/imgui_impl_glfw_gl3.cpp
        Debugger/Imgui/imgui_impl_glfw_gl3.h
        Debugger/Imgui/imgui_internal.h
        Debugger/Imgui/imgui_widgets.cpp
        Debugger/Imgui/imstb_rectpack.h
        Debugger/Imgui/imstb_textedit.h
        Debugger/Imgui/imstb_truetype.h
        Debugger/Imgui/stb_image.h
        Debugger/CartridgeDebugger.cpp
        Debugger/CartridgeDebugger.h
        Debugger/CPUDebugger.cpp
        Debugger/CPUDebugger.h
        Debugger/Debugger.cpp
        Debugger/Debugger.h
        Debugger/MemoryDebugger.cpp
        Debugger/MemoryDebugger.h
        Debugger/VideoDebugger.cpp
        Debugger/VideoDebugger.h
        Memory/MBC1.cpp
        Memory/MBC1.h
        Memory/MBC2.cpp
        Memory/MBC2.h
        Memory/MBC3.cpp
        Memory/MBC3.h
        Memory/MemoryChip.h
        Memory/RomOnly.cpp
        Memory/RomOnly.h
        Audio.cpp
        Audio.h
        Cartridge.cpp
        Cartridge.h
        CPU.cpp
        CPU.h
        GameBoy.cpp
        GameBoy.h
        Joypad.cpp
        Joypad.h
	main.cpp
        Memory.cpp
        Memory.h
        Types.h
        Utils.h
        Video.cpp
        Video.h)

target_link_libraries(PatBoy ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SDL2_LIBRARIES} GLEW::GLEW glfw)
