#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>
#include "App.h"

/*
* TODO:
* Make Logging System
*/

void Closed(GLFWwindow* window) {
    std::cout << "Closeda";
}

void Update(GLFWwindow* wind) {

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

    Application.Run();

    return 0;
}