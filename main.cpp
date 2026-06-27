#include <iostream>
#include <thread>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include "Session.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int main() {
	
	if (!glfwInit()) {
		std::cout << "Initialization failed.\n";
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	while (!glfwWindowShouldClose(window)) {
		int duration = 0;
		int bufSize = 64;
		char buf[] = "";
		int h = 1, m = 24, s = 39;

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Timer");
		ImGui::InputInt("Duration (min)", &duration);
		ImGui::InputText("Label", buf, bufSize);
		ImGui::Button("Start");
		ImGui::Text("Remaining: %d:%02d:%02d", h, m, s);
		ImGui::End();

		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}