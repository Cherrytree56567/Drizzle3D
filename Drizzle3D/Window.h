#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Events.h"

namespace Drizzle3D {

	class Window {
	public:
		Window(Events* dispatch, char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);
		~Window();

		GLFWwindow* returnwindow() { return window; };
		int returnWidth() { return winwidth; }
		int returnHeight() { return winheight; }
		int returnX() { return winx; }
		int returnY() { return winy; }

		void ProcessEvents();
		void Render();

		Events* dispatcher;
	private:
		GLFWwindow* window = NULL;
		int winwidth;
		int winheight;
		int winx;
		int winy;
	};

}