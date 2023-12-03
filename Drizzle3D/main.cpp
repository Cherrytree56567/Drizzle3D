#include <iostream>
#include "Window.h"

void moved(GLFWwindow* window, int xpos, int ypos) {
    std::cout << xpos << " " << ypos;
}

int main() {
    Drizzle3D::Window D3DWindow;

    D3DWindow.CreateWindow();

    D3DWindow.evdis.WindowMoved = moved;

    while (D3DWindow.Update()) {

    }

    D3DWindow.~Window();

    return 0;
}