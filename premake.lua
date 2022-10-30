workspace "PatBoy"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	location "generated"
	objdir "temp/%{cfg.platform}_%{cfg.buildcfg}"

filter { "platforms:Win64" }
	system "Windows"
	architecture "x86_64"


-- EXTERNAL LIBRARIES

include "libs/SDL2/premake.lua"

include "libs/glfw/premake.lua"


-- PATBOY PROJECT

project "patboy"
	kind "WindowedApp"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	links { "sdl2", "glew32", "glut32", "glfw", "OpenGL32" }

    libdirs
	{
        "libs/glew/lib"
    }

	includedirs
	{
		"libs/SDL2/include",
		"libs/glew/include",
		"libs/glfw/include"
	}

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	defines
	{
		"GLUT_BUILDING_LIB"
	}

	filter "configurations.Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations.Release"
		defines { "RELEASE" }
		optimize "On"

	filter "system:Windows"
		postbuildcommands
		{
	  		"{COPYFILE} ..\\libs\\glew\\lib\\glew32.dll ..\\bin\\%{cfg.buildcfg}\\glew32.dll"
		}