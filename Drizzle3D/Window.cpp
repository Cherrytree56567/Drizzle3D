#include "Window.h"

namespace Drizzle3D {
	Events disp;
	LRESULT CALLBACK WindowProcess(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam) {
		switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			disp.DispatchEvent(EventType::WindowResize, hWnd, message, wparam, lparam);
			break;

		case WM_SETFOCUS:
			disp.DispatchEvent(EventType::WindowFocus, hWnd, message, wparam, lparam);
			break;

		case WM_KILLFOCUS:
			disp.DispatchEvent(EventType::WindowLostFocus, hWnd, message, wparam, lparam);
			break;

		case WM_MOVE:
			disp.DispatchEvent(EventType::WindowMoved, hWnd, message, wparam, lparam);
			break;

		case WM_CLOSE:
			disp.DispatchEvent(EventType::WindowClose, hWnd, message, wparam, lparam);
			break;

		case WM_KEYDOWN:
			disp.DispatchEvent(EventType::KeyPressed, hWnd, message, wparam, lparam);
			break;

		case WM_KEYUP:
			disp.DispatchEvent(EventType::KeyReleased, hWnd, message, wparam, lparam);
			break;

		case WM_LBUTTONDOWN:
			disp.DispatchEvent(EventType::MouseLeftButtonPressed, hWnd, message, wparam, lparam);
			break;

		case WM_LBUTTONUP:
			disp.DispatchEvent(EventType::MouseLeftButtonReleased, hWnd, message, wparam, lparam);
			break;

		case WM_RBUTTONDOWN:
			disp.DispatchEvent(EventType::MouseRightButtonPressed, hWnd, message, wparam, lparam);
			break;

		case WM_RBUTTONUP:
			disp.DispatchEvent(EventType::MouseRightButtonReleased, hWnd, message, wparam, lparam);
			break;

		case WM_MOUSEMOVE:
			disp.DispatchEvent(EventType::MouseMoved, hWnd, message, wparam, lparam);
			break;

		case WM_MOUSEWHEEL:
			disp.DispatchEvent(EventType::MouseScrolled, hWnd, message, wparam, lparam);
			break;
		}

		return DefWindowProc(hWnd, message, wparam, lparam);
	}
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

		wcex.lpfnWndProc = WindowProcess;

		RegisterClassEx(&wcex);

		size_t charLength = std::strlen(title) + 1;

		wchar_t* wcharString = new wchar_t[charLength];
		mbstowcs(wcharString, title, charLength);

		hWnd = CreateWindow(WindowClass, wcharString, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, GetModuleHandle(NULL), nullptr);
		if (!hWnd) {
			MessageBox(0, L"[Drizzle3D::Window] Error: Failed to Create Window!.", 0, 0);
		}
		delete[] wcharString;

		ShowWindow(hWnd, SW_SHOW);

		pRender = std::make_unique<Renderer>(hWnd);
	}

	bool Window::Update() {
		MSG msg{};
		if (msg.message != WM_QUIT) {
			disp = dispatcher;
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return true;
		}
		else {
			return false;
		}
	}

	void SetTitle(char* title, HWND hWnd) {
		int wideCharLength = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);

		// Allocate memory for the wide character string
		wchar_t* wideCharString = new wchar_t[wideCharLength];

		// Convert the multibyte character string to wide character string
		MultiByteToWideChar(CP_UTF8, 0, title, -1, wideCharString, wideCharLength);
		SetWindowText(hWnd, wideCharString);
	}

	void SetIcon(char* filepath, HWND hwnd) {
		int wideCharLength = MultiByteToWideChar(CP_UTF8, 0, filepath, -1, NULL, 0);

		// Allocate memory for the wide character string
		wchar_t* wideCharString = new wchar_t[wideCharLength];

		// Convert the multibyte character string to wide character string
		MultiByteToWideChar(CP_UTF8, 0, filepath, -1, wideCharString, wideCharLength);
		HICON hIcon = (HICON)LoadImage(NULL, wideCharString, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

		if (hIcon != NULL) {
			// Set the icon for both big and small icons
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		}
	}
}