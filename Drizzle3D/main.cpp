/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include <GLAD/glad.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "App.h"

void ImGUICode(std::shared_ptr<Drizzle3D::ImGuiLayer> rend) {
    float a = 0;
    ImGui::SliderFloat("Test Slider", &a, 0.0f, 360.0f);

    if (ImGui::Button("Test Button")) {
        std::cout << "Test Button\n";
    }
}

void Update(Drizzle3D::App* app) {

}

Drizzle3D_API int main() {

    /*
    * NOTE:
    * Key Released and Mouse Released
    */
    Drizzle3D::App app;

    app.update = Update;
    app.ImguiLayer()->code = ImGUICode;

    app.Run();

    return 0;
}