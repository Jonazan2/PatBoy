#pragma once

#include "../../Types.h"

struct GLFWwindow;

typedef struct {
	unsigned int id;
	int width;
	int height;
	void* buffer;
} Texture;


IMGUI_API bool			ImGui_ImplGlfwGL3_Init(GLFWwindow* window, bool install_callbacks);
IMGUI_API ImTextureID   ImGui_ImplGlfwGL3_CreateTexture(Texture& buffer);
IMGUI_API void			ImGui_ImplGlfwGL3_Bind_Textures();
// IMGUI_API void			ImGui_ImplGlfwGL3_UpdateTexture(ImTextureID textureId, void* buffer);

IMGUI_API void			ImGui_ImplGlfwGL3_Shutdown();
IMGUI_API void			ImGui_ImplGlfwGL3_NewFrame();

IMGUI_API void			ImGui_ImplGlfwGL3_InvalidateDeviceObjects();
IMGUI_API bool			ImGui_ImplGlfwGL3_CreateDeviceObjects();

IMGUI_API void			ImGui_ImplGlfwGL3_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
IMGUI_API void			ImGui_ImplGlfwGL3_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
IMGUI_API void			ImGui_ImplGlfwGL3_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
IMGUI_API void			ImGui_ImplGlfwGL3_CharCallback(GLFWwindow* window, unsigned int c);

IMGUI_API void			ImGui_ImplGlfwGL3_RenderDrawLists(ImDrawData* draw_data);
