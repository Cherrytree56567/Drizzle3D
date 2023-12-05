#include <windows.h>
#include "Application.h"

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

void Updat(Drizzle3D::Application* app) {
    const float c = sin(app->timer.Peek()) / 2.0f + 0.5f;
    app->D3DWindow.GetRenderer().ClearBuffer(c, c, 1.0f);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {

    Drizzle3D::Application app;

    app.D3DWindow.dispatcher.AddEventListener(Drizzle3D::EventType::KeyPressed, KeyD);
    app.D3DWindow.dispatcher.AddEventListener(Drizzle3D::EventType::WindowResize, resix);
    app.D3DWindow.dispatcher.AddEventListener(Drizzle3D::EventType::MouseLeftButtonPressed, MouseD);

    app.Update = Updat;

    app.Run();
    return 0;
}
