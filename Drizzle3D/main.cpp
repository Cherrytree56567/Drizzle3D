#include <windows.h>
#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {

    Drizzle3D::Window D3DWindow((char*)"New Window");

    while (D3DWindow.Update()) {

    }
    return 0;
}
