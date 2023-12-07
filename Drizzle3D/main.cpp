#include <GLAD/glad.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>
#include "App.h"

/*
* NOTE:
* Allow changing the order of Layers.
*/

void Closed(GLFWwindow* window) {
    std::cout << "Closeda";
}

void Update(GLFWwindow* wind) {

}

void ImGUICode(Drizzle3D::ImGuiLayer* rend) {
    char* buf = (char*)"";
    float f = 0;
    ImGui::Text("Hello, world %d", 123);
    if (ImGui::Button("Save")) {}
    ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
}

int main() {
    Drizzle3D::App Application;

    Application.dispatcher()->AddEventListener(Drizzle3D::EventType::WindowClose, [](GLFWwindow* window) {
        std::cout << "Closed";
    });

    Application.dispatcher()->AddEventListener(Drizzle3D::EventType::WindowMoved, [](GLFWwindow* window) {
        std::cout << "Moved\n";
        });

    Application.dispatcher()->AddEventListener(Drizzle3D::EventType::WindowClose, Closed);

    Application.dispatcher()->AddEventListener(Drizzle3D::EventType::AppRender, Update);

    Application.ImguiLayer()->code = ImGUICode;

    Application.Run();

    return 0;
}