/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <GLAD/glad.h>
#include <vector>
#include <iostream>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "AppEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "base.h"

namespace Drizzle3D {

	class Drizzle3D_API Window {
	public:
		Window(EventDispatcher* dispatch, char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);
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

		EventDispatcher* dispatcher;
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
		double lastSMouseX = 0.0;
		double lastSMouseY = 0.0;
	};

}