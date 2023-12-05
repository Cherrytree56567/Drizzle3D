#pragma once
#include "Window.h"
#include "Timer.h"

namespace Drizzle3D {
	class Application {
	public:
		typedef void (*UpdateCallback)(Application* app);
		Application(char* title = (char*)"New Drizzle3D Game", int width = 800, int height = 600, HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW)) : D3DWindow(title, width, height, LoadIcon(0, IDI_APPLICATION), LoadIcon(0, IDI_APPLICATION), cursor) {}

		void Run();
		UpdateCallback Update = [](Application* app) {};
		void Render();

		Drizzle3D::Window D3DWindow;
		Timer timer;
	};
}