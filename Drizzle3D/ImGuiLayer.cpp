/*
***********************************************************************
*                                                                     *
* Drizzle3D � 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
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
        switch (renderingAPI) {
        case RenderingAPI::OpenGL:
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            // ---- //
            ImGui::NewFrame();

            code(igui);
            // ---- //
            break;

        case RenderingAPI::Vulkan:
            log.Warning("ImGui Vulkan Frames Not Supported.");
            break;
        }
        // TODO: After Vulkan Implementation transfer `ImGui::NewFrame();` and `code(igui);` here

        

        IterateSliderFloat();
        SliderFloats.clear();

        // Rendering
        /*
        * TODO: After Vulkan Implementation transfer ```ImGui::Render();
                                                        int display_w, display_h;
                                                        glfwGetFramebufferSize(pWindow->returnwindow(), &display_w, &display_h);``` here.
        */
        switch (renderingAPI) {
        case RenderingAPI::OpenGL:
            // ---- //
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(pWindow->returnwindow(), &display_w, &display_h);
            // ----- //
            glViewport(0, 0, display_w, display_h);
            break;

        case RenderingAPI::Vulkan:
            log.Warning("ImGui Vulkan Viewporting Not Supported.");
            break;
        }

        switch (renderingAPI) {
        case RenderingAPI::OpenGL:
            // Render ImGui
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            break;

        case RenderingAPI::Vulkan:
            log.Warning("ImGui Vulkan Rendering Not Supported.");
            break;
        }
	}

    void ImGuiLayer::OnAttach() {

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        imguiContext = ImGui::GetCurrentContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        switch (renderingAPI) {
        case RenderingAPI::OpenGL:
            // Setup ImGui GLFW binding
            ImGui_ImplGlfw_InitForOpenGL(pWindow->returnwindow(), true);
            // Setup ImGui OpenGL binding
            ImGui_ImplOpenGL3_Init("#version 330 core");
            break;

        case RenderingAPI::Vulkan:
            log.Warning("ImGui Vulkan Initialization Not Supported.");
            break;
        }
    }
}