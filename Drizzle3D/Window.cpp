#include "Window.h"


namespace Drizzle3D {
	Window::Window(Events* dispatch, char* WindowName, int width, int height) : dispatcher(dispatch) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(width, height, WindowName, NULL, NULL);
		if (window == NULL) {
			std::cout << "[Drizzle3D::Core::Window] Error: Failed to create GLFW window." << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		winwidth = width;
		winheight = height;
		glfwGetWindowPos(window, &winx, &winy);
	}

	Window::~Window() {
		glfwTerminate();
	}

	void Window::ProcessEvents() {
		glfwSwapBuffers(returnwindow());
		glfwPollEvents();
		int newwidth, newheight;
		glfwGetFramebufferSize(window, &newwidth, &newheight);

		if (newwidth != winwidth || newheight != winheight) {
			winwidth = newwidth;
			winheight = newheight;
			dispatcher->DispatchEvent(EventType::WindowResize, window);
		}

		if (glfwWindowShouldClose(window)) {
			dispatcher->DispatchEvent(EventType::WindowClose, window);
		}

		if (glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE) {
			dispatcher->DispatchEvent(EventType::WindowFocus, window);
		}
		else {
			dispatcher->DispatchEvent(EventType::WindowLostFocus, window);
		}

		int x, y;
		glfwGetWindowPos(window, &x, &y);
		if (x != winx) {
			winx = x;
			winy = y;
			dispatcher->DispatchEvent(EventType::WindowMoved, window);
		} else if (y != winy) {
			winx = x;
			winy = y;
			dispatcher->DispatchEvent(EventType::WindowMoved, window);
		}
	}

	void Window::Render() {
		dispatcher->DispatchEvent(EventType::AppRender, window);
	}
}