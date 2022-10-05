workspace "PatBoy"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	location "generated"
	objdir "temp/%{cfg.platform}_%{cfg.buildcfg}"

filter { "platforms:Win64" }
	system "Windows"
	architecture "x86_64"

-- EXTERNAL LIBRARIES

project "sdl2"
	kind "StaticLib"
	language "C++"

	links
	{
		"setupapi",
		"winmm",
		"imm32",
		"version",
	}

	includedirs
	{
		"libs/SDL2/include"
	}

	files
	{
		"libs/SDL2/include/**.h",
		"libs/SDL2/src/*.h",
		"libs/SDL2/src/*.c",
		"libs/SDL2/src/atomic/*.h",
		"libs/SDL2/src/atomic/*.c",
		"libs/SDL2/src/audio/*.h",
		"libs/SDL2/src/audio/*.c",
		"libs/SDL2/src/audio/disk/*.h",
		"libs/SDL2/src/audio/disk/*.c",
		"libs/SDL2/src/audio/dummy/*.h",
		"libs/SDL2/src/audio/dummy/*.c",
		"libs/SDL2/src/cpuinfo/*.h",
		"libs/SDL2/src/cpuinfo/*.c",
		"libs/SDL2/src/dynapi/*.h",
		"libs/SDL2/src/dynapi/*.c",
		"libs/SDL2/src/events/*.h",
		"libs/SDL2/src/events/*.c",
		"libs/SDL2/src/file/*.h",
		"libs/SDL2/src/file/*.c",
		"libs/SDL2/src/haptic/*.h",
		"libs/SDL2/src/haptic/*.c",
		"libs/SDL2/src/haptic/dummy/*.h",
		"libs/SDL2/src/haptic/dummy/*.c",
		"libs/SDL2/src/hidapi/*.h",
		"libs/SDL2/src/hidapi/*.c",
		"libs/SDL2/src/joystick/*.h",
		"libs/SDL2/src/joystick/*.c",
		"libs/SDL2/src/joystick/dummy/*.h",
		"libs/SDL2/src/joystick/dummy/*.c",
		"libs/SDL2/src/joystick/hidapi/*.h",
		"libs/SDL2/src/joystick/hidapi/*.c",
		"libs/SDL2/src/joystick/virtual/*.h",
		"libs/SDL2/src/joystick/virtual/*.c",
		"libs/SDL2/src/libm/*.h",
		"libs/SDL2/src/libm/*.c",
		"libs/SDL2/src/locale/*.h",
		"libs/SDL2/src/locale/*.c",
		"libs/SDL2/src/misc/*.h",
		"libs/SDL2/src/misc/*.c",
		"libs/SDL2/src/power/*.h",
		"libs/SDL2/src/power/*.c",
		"libs/SDL2/src/render/*.h",
		"libs/SDL2/src/render/*.c",
		"libs/SDL2/src/render/software/*.h",
		"libs/SDL2/src/render/software/*.c",
		"libs/SDL2/src/sensor/*.h",
		"libs/SDL2/src/sensor/*.c",
		"libs/SDL2/src/sensor/dummy/*.h",
		"libs/SDL2/src/sensor/dummy/*.c",
		"libs/SDL2/src/stdlib/*.h",
		"libs/SDL2/src/stdlib/*.c",
		"libs/SDL2/src/thread/*.h",
		"libs/SDL2/src/thread/*.c",
		"libs/SDL2/src/thread/generic/SDL_syscond.h",
		"libs/SDL2/src/thread/generic/SDL_syscond.c",
		"libs/SDL2/src/timer/*.h",
		"libs/SDL2/src/timer/*.c",
		"libs/SDL2/src/video/*.h",
		"libs/SDL2/src/video/*.c",
		"libs/SDL2/src/video/dummy/*.h",
		"libs/SDL2/src/video/dummy/*.c",
	}

	filter "system:Windows"
		links
		{
			"setupapi"
		}

		files
		{
			"libs/SDL2/src/main/windows/SDL_windows_main.c",
			"libs/SDL2/src/audio/directsound/*.h",
			"libs/SDL2/src/audio/directsound/*.c",
			"libs/SDL2/src/audio/winmm/*.h",
			"libs/SDL2/src/audio/winmm/*.c",
			"libs/SDL2/src/audio/wasapi/*.h",
			"libs/SDL2/src/audio/wasapi/*.c",
			"libs/SDL2/src/core/windows/*.h",
			"libs/SDL2/src/core/windows/*.c",
			"libs/SDL2/src/filesystem/windows/*.h",
			"libs/SDL2/src/filesystem/windows/*.c",
			"libs/SDL2/src/haptic/windows/*.h",
			"libs/SDL2/src/haptic/windows/*.c",
			"libs/SDL2/src/joystick/windows/*.h",
			"libs/SDL2/src/joystick/windows/*.c",
			"libs/SDL2/src/loadso/windows/*.h",
			"libs/SDL2/src/loadso/windows/*.c",
			"libs/SDL2/src/locale/windows/*.h",
			"libs/SDL2/src/locale/windows/*.c",
			"libs/SDL2/src/misc/windows/*.h",
			"libs/SDL2/src/misc/windows/*.c",
			"libs/SDL2/src/power/windows/*.h",
			"libs/SDL2/src/power/windows/*.c",
			"libs/SDL2/src/render/direct3d/*.h",
			"libs/SDL2/src/render/direct3d/*.c",
			"libs/SDL2/src/render/direct3d11/*.h",
			"libs/SDL2/src/render/direct3d11/*.c",
			"libs/SDL2/src/render/opengl/*.h",
			"libs/SDL2/src/render/opengl/*.c",
			"libs/SDL2/src/render/opengles2/*.h",
			"libs/SDL2/src/render/opengles2/*.c",
			"libs/SDL2/src/sensor/windows/*.h",
			"libs/SDL2/src/sensor/windows/*.c",
			"libs/SDL2/src/thread/windows/*.h",
			"libs/SDL2/src/thread/windows/*.c",
			"libs/SDL2/src/timer/windows/*.h",
			"libs/SDL2/src/timer/windows/*.c",
			"libs/SDL2/src/video/khronos/vulkan/*.h",
			"libs/SDL2/src/video/windows/*.h",
			"libs/SDL2/src/video/windows/*.c",
			"libs/SDL2/src/video/yuv2rgb/*.h",
			"libs/SDL2/src/video/yuv2rgb/*.c",
		}

		defines
		{
			"HAVE_LIBC"
		}

project "glfw"
	kind "StaticLib"
	language "C"
	
	includedirs { "libs/glfw/include/" }

	files
	{
	    "libs/glfw/include/GLFW/glfw3.h",
    	"libs/glfw/include/GLFW/glfw3native.h",
		"libs/glfw/src/glfw_config.h",
		"libs/glfw/src/context.c",
		"libs/glfw/src/init.c",
		"libs/glfw/src/input.c",
		"libs/glfw/src/monitor.c",
		"libs/glfw/src/vulkan.c",
		"libs/glfw/src/window.c"
	}

	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
			"libs/glfw/src/win32_init.c",
			"libs/glfw/src/win32_joystick.c",
			"libs/glfw/src/win32_monitor.c",
			"libs/glfw/src/win32_time.c",
			"libs/glfw/src/win32_thread.c",
			"libs/glfw/src/win32_window.c",
			"libs/glfw/src/wgl_context.c",
			"libs/glfw/src/egl_context.c",
			"libs/glfw/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


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