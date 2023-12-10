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
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "[Drizzle3D::Core::Window] Error: Failed to Load GL Loader." << std::endl;
			exit(-1);
		}
		winwidth = width;
		winheight = height;
		glfwGetWindowPos(window, &winx, &winy);
		glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		glEnable(GL_DEPTH_TEST);
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

		bool keyp = false;
		bool keyr = false;
		for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
			if (glfwGetKey(window, key) == GLFW_PRESS) {
				keyr = false;
				key_codes.push_back(key);
			}
			if (glfwGetKey(window, key) == GLFW_RELEASE) {
				keyr = true;
				keyRel_codes.push_back(key);
			}
		}
		if (keyp) {
			dispatcher->DispatchEvent(EventType::KeyPressed, window);
		}

		int leftMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (leftMouseButtonState == GLFW_PRESS && !wasLeftMouseButtonPressed) {
			wasLeftMouseButtonPressed = true;
			dispatcher->DispatchEvent(EventType::MouseLeftButtonPressed, window);
		}
		else if (leftMouseButtonState == GLFW_RELEASE && wasLeftMouseButtonPressed) {
			wasLeftMouseButtonPressed = false;
			dispatcher->DispatchEvent(EventType::MouseLeftButtonReleased, window);
		}

		int rightMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (rightMouseButtonState == GLFW_PRESS && !wasRightMouseButtonPressed) {
			wasRightMouseButtonPressed = true;
			dispatcher->DispatchEvent(EventType::MouseRightButtonPressed, window);
		}
		else if (rightMouseButtonState == GLFW_RELEASE && wasRightMouseButtonPressed) {
			wasRightMouseButtonPressed = false;
			dispatcher->DispatchEvent(EventType::MouseRightButtonReleased, window);
		}

		// Check mouse movement
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		if (mouseX != lastMouseX || mouseY != lastMouseY) {
			lastMouseX = mouseX;
			lastMouseY = mouseY;
			dispatcher->DispatchEvent(EventType::MouseMoved, window);
		}
	}

	void Window::Render() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}