#include <iostream>
#include <thread>
#include <memory>
#include <string_view>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include "Session.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const int LABEL_BUFFER_SIZE = 64;

int main() {
	char label[LABEL_BUFFER_SIZE] = "";
	std::unique_ptr<Session> active_session = nullptr; 
	int duration = 0;

	if (!glfwInit()) {
		std::cout << "Initialization failed.\n";
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Pomodoro Timer", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2.0f;

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		int w, h;
		bool p_open = true;
		glfwGetWindowSize(window, &w, &h);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
		ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::Begin("Timer", &p_open,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

		if (active_session == nullptr) {
			ImGui::InputInt("Duration (min)", &duration);
			ImGui::InputText("Label", label, LABEL_BUFFER_SIZE);
			if (ImGui::Button("Start")) {
				active_session = std::make_unique<Session>(
					Session(
						std::chrono::minutes(duration), std::string_view(label)));
			}
		}
		else {
			auto [h, m, s] = active_session->getRemainingTime();
			ImGui::Text("Remaining: %d:%02d:%02d", h, m, s);
			if (active_session->isTimerComplete() || ImGui::Button("Stop")) {
				active_session = nullptr;
				duration = 0;
				label[0] = '\0';
			}
		}
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