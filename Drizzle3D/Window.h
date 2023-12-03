#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include "Events.h"

namespace Drizzle3D {

	class Window {
	public:
		Window();
		void CreateWindow(int width = 800, int height = 600, char* WindowName = (char*)"New Drizzle3D Game");
		GLFWwindow* returnwindow() { return window; };
		~Window();
		EventDispatcher evdis;
		typedef void (*Function)();
		void focus_callback(GLFWwindow* window, int focused) {
			if (focused == GLFW_TRUE) {
				evdis.OnWindowFocus(window);
			}
			else {
				evdis.OnWindowLostFocus(window);
			}
		}
		bool Update() {
			if (!glfwWindowShouldClose(returnwindow())) {
				glfwSwapBuffers(returnwindow());
				glfwPollEvents();
				return true;
			} else {
				evdis.OnWindowClose(window);
				return false;
			}
		}
	private:
		GLFWwindow* window = NULL;
	};

}