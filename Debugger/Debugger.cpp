#include "Imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

class Debugger{
public:
	Debugger() {
		clear_color = ImColor(114, 144, 154);
	}

	int startDebugger()
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(400, 400, "PatBoy Debugger", NULL, NULL);
		glfwMakeContextCurrent(window);

		GLint GlewInitResult = glewInit();
		if (GLEW_OK != GlewInitResult)
		{
			printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
			exit(EXIT_FAILURE);
		}

		// Setup ImGui binding
		ImGui_ImplGlfwGL3_Init(window, true);
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Text("Hello, world!");
		ImGui::Button("Continue");
		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	void update() {
		glfwPollEvents();

		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Text("Hello, world!");
		if (ImGui::Button("Continue")) {

			// Rendering
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui::Render();
			glfwSwapBuffers(window);
		}
	}

	void closeDebugger() {
		ImGui_ImplGlfwGL3_Shutdown();
		glfwTerminate();
	}

private:
	GLFWwindow* window;
	ImVec4 clear_color;
};