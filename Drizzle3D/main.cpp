#include <windows.h>
#include "Window.h"

void resix() {
    MessageBox(0, L"Resize", 0, 0);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {

    Drizzle3D::Window D3DWindow((char*)"New Window");

    D3DWindow.dispatcher.AddEventListener(Drizzle3D::EventType::WindowResize, resix);

    while (D3DWindow.Update()) {

    }
    return 0;
}
