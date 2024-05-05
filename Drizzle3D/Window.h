/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include "AppEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "Object.h"
#include "base.h"
#include "ObjectWindow.h"

namespace Drizzle3D {

	class Window {
	public:
		Drizzle3D_API Window(RenderingAPI rAPI, std::shared_ptr<EventDispatcher> dispatch, char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);
		Drizzle3D_API ~Window();

		Drizzle3D_API GLFWwindow* returnwindow() { return window; };
		Drizzle3D_API int returnWidth() { return winwidth; }
		Drizzle3D_API int returnHeight() { return winheight; }
		Drizzle3D_API int returnX() { return winx; }
		Drizzle3D_API int returnY() { return winy; }
		Drizzle3D_API std::vector<int> returnKeyPressedCodes() { return key_codes; }
		Drizzle3D_API void clearKeyCodes() { key_codes.clear(); }
		Drizzle3D_API std::vector<int> returnKeyReleasedCodes() { return keyRel_codes; }
		Drizzle3D_API void clearKeyReleasedCodes() { keyRel_codes.clear(); }
		Drizzle3D_API double returnMouseX() { return lastMouseX; }
		Drizzle3D_API double returnMouseY() { return lastMouseY; }
		Drizzle3D_API void AddRLayer(std::any rL) { rLayer = rL; }

		Drizzle3D_API void ProcessEvents();
		Drizzle3D_API void Render();

		std::shared_ptr<EventDispatcher> dispatcher;
	private:
		GLFWwindow* window = NULL;
		RenderingAPI renderingAPI;
		std::any rLayer;
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
