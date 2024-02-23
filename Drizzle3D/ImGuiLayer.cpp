/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "ImGuiLayer.h"

namespace Drizzle3D {
    void ImGuiLayer::GUISliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, int flags) {
        SliderFloat sf = { label, v, v_min, v_max, format, flags };
        SliderFloats.push_back(sf);
    }

    void ImGuiLayer::IterateSliderFloat() {
        for (auto& sf : SliderFloats) {
            if (sf.format == NULL) {
                if (sf.flags == NULL) {
                    ImGui::SliderFloat(sf.label, sf.v, sf.v_min, sf.v_max);
                }
            }
            else {
                ImGui::SliderFloat(sf.label, sf.v, sf.v_min, sf.v_max, sf.format, sf.flags);
            }
        }
    }

	void ImGuiLayer::Render() {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Begin ImGui window
        ImGui::Begin("ImGui Window");

        // Add ImGui button
        if (ImGui::Button("Click Me")) {
            // Button click action
        }

        // End ImGui window
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(pWindow->returnwindow(), &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);

        // Render ImGui
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

    void ImGuiLayer::OnAttach() {

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        imguiContext = ImGui::GetCurrentContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        // Setup ImGui GLFW binding
        ImGui_ImplGlfw_InitForOpenGL(pWindow->returnwindow(), true);

        // Setup ImGui OpenGL binding
        ImGui_ImplOpenGL3_Init("#version 330 core");
        ImGui_ImplDX11_Init(pWindow->returnDX11Device(), pWindow->returnDX11DeviceContext());
    }
}