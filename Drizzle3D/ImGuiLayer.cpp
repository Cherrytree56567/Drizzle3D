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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}