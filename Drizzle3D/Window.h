#pragma once
#include <GLAD/glad.h>
#include <vector>
#include <iostream>
#include "Events.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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
		std::vector<int> returnKeyPressedCodes() { return key_codes; }
		void clearKeyCodes() { key_codes.clear(); }
		std::vector<int> returnKeyReleasedCodes() { return keyRel_codes; }
		void clearKeyReleasedCodes() { keyRel_codes.clear(); }
		double returnMouseX() { return lastMouseX; }
		double returnMouseY() { return lastMouseY; }

		void ProcessEvents();
		void Render();

		Events* dispatcher;
	private:
		GLFWwindow* window = NULL;
		int winwidth;
		int winheight;
		int winx;
		int winy;
		std::vector<int> key_codes;
		std::vector<int> keyRel_codes;
		bool wasLeftMouseButtonPressed = false;
		bool wasRightMouseButtonPressed = false;
		double lastMouseX = 0.0;
		double lastMouseY = 0.0;
	};

}