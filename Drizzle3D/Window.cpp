#include "Window.h"

namespace Drizzle3D {
	Window::Window() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	void Window::CreateWindow(int width, int height, char* WindowName) {
		window = glfwCreateWindow(width, height, WindowName, NULL, NULL);
		if (window == NULL) {
			std::cout << "[Drizzle3D::Window] Error: Failed to create GLFW window." << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, evdis.WindowResize);
		glfwSetWindowPosCallback(window, evdis.WindowMoved);
	}

	Window::~Window() {
		glfwTerminate();
	}
}