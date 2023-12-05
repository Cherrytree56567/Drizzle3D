#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstring>
#include <cwchar>
#include <memory>
#include "Renderer.h"
#include "Events.h"

namespace Drizzle3D {
	void SetTitle(char* title, HWND hwnd);
	void SetIcon(char* filepath, HWND hwnd);
	class Window {
	public:
		Window(char* title = (char*)"New Drizzle3D Game", int width = 800, int height = 600, HICON TBIcon = LoadIcon(0, IDI_APPLICATION), HICON WICON = LoadIcon(0, IDI_APPLICATION), HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW));
		bool Update();
		void Render();
		Renderer& GetRenderer() { return *pRender; }
		HWND GetHandle() { return hWnd; }
		Events dispatcher;
	private:
		std::unique_ptr<Renderer> pRender;
		WCHAR WindowClass[256];
		HWND hWnd;
	};
}