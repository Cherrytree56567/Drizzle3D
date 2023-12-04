#include "Window.h"

namespace Drizzle3D {
	Window::Window(char* title, int width, int height, HICON TBICON, HICON WICON, HCURSOR cursor) {
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;

		wcex.hCursor = cursor;
		wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

		wcex.hIcon = WICON; // Icon on Window
		wcex.hIconSm = TBICON; // Icon on Taskbar

		wcscpy_s(WindowClass, TEXT("Drizzle3D_Game"));
		wcex.lpszClassName = WindowClass;

		wcex.lpszMenuName = nullptr;

		wcex.hInstance = GetModuleHandle(NULL);

		wcex.lpfnWndProc = DefWindowProc;

		RegisterClassEx(&wcex);

		size_t charLength = std::strlen(title) + 1;

		wchar_t* wcharString = new wchar_t[charLength];
		mbstowcs(wcharString, title, charLength);

		HWND hWnd = CreateWindow(WindowClass, wcharString, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, GetModuleHandle(NULL), nullptr);
		if (!hWnd) {
			MessageBox(0, L"[Drizzle3D::Window] Error: Failed to Create Window!.", 0, 0);
		}
		delete[] wcharString;

		ShowWindow(hWnd, SW_SHOW);
	}

	bool Window::Update() {
		MSG msg = { 0 };
		if (msg.message != WM_QUIT) {
			// If there are Window messages then process them.
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			return true;
		} else {
			return false;
		}
	}
}