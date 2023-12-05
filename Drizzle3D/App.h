#pragma once
#include "Window.h"

namespace Drizzle3D {
	class App {
	public:
		App(char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);

		void Run();

		Window* window() { return &D3DWindow; }
		Events* dispatcher() { return &dispatch; }

	private:
		Window D3DWindow;
		Events dispatch;
	};
}