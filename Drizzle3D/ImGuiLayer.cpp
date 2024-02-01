/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "ImGuiLayer.h"

namespace Drizzle3D {
	void ImGuiLayer::Render() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        code(this);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(pWindow->returnwindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        // Render ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

    void ImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Setup ImGui GLFW binding
        ImGui_ImplGlfw_InitForOpenGL(pWindow->returnwindow(), true);

        // Setup ImGui OpenGL binding
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }
}