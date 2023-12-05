#include <windows.h>
#include "Window.h"

void resix(HWND hwnd, UINT, WPARAM wparam, LPARAM lparam) {
    Drizzle3D::SetTitle((char*)"Resized", hwnd);
}

void KeyD(HWND hwnd, UINT, WPARAM KeyCode, LPARAM) {
    if (KeyCode == 'F') {
        Drizzle3D::SetTitle((char*)"Key F pressed", hwnd);
    }
}

void MouseD(HWND hwnd, UINT, WPARAM KeyCode, LPARAM) {
    Drizzle3D::SetTitle((char*)"Mouse Left Down", hwnd);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {

    Drizzle3D::Window D3DWindow((char*)"New Window");

    D3DWindow.dispatcher.AddEventListener(Drizzle3D::EventType::KeyPressed, KeyD);
    D3DWindow.dispatcher.AddEventListener(Drizzle3D::EventType::WindowResize, resix);
    D3DWindow.dispatcher.AddEventListener(Drizzle3D::EventType::MouseLeftButtonPressed, MouseD);

    while (D3DWindow.Update()) {

    }
    return 0;
}
