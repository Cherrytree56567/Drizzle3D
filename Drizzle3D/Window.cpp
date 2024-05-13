/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "Window.h"

namespace Drizzle3D {
	double xffset, yffset = 0.0f;
	bool a = false;
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		a = true;
		xffset = xoffset;
		yffset = yoffset;
	}
	Window::Window(RenderingAPI rAPI, std::shared_ptr<EventDispatcher> dispatch, char* WindowName, int width, int height) : dispatcher(dispatch), renderingAPI(rAPI) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		if (renderingAPI == RenderingAPI::OpenGL) {
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		} else if (renderingAPI = RenderingAPI::Vulkan) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}
		window = glfwCreateWindow(width, height, WindowName, NULL, NULL);
		if (window == NULL) {
			std::cout << "[Drizzle3D::Core::Window] Error: Failed to create GLFW window." << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		if (renderingAPI == RenderingAPI::OpenGL) {
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cout << "[Drizzle3D::Core::Window] Error: Failed to Load GL Loader." << std::endl;
				exit(-1);
			}
		}
		winwidth = width;
		winheight = height;
		glfwGetWindowPos(window, &winx, &winy);
		glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		
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
			std::unique_ptr<WindowResizeEvent> at = std::make_unique<WindowResizeEvent>((unsigned int)winwidth, (unsigned int)winheight);
			dispatcher->AddEvent(std::move(at));
		}

		if (glfwWindowShouldClose(window)) {
			std::unique_ptr<WindowCloseEvent> at = std::make_unique<WindowCloseEvent>();
			dispatcher->AddEvent(std::move(at));
		}

		if (glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE) {
			std::unique_ptr<WindowFocusEvent> at = std::make_unique<WindowFocusEvent>(true);
			dispatcher->AddEvent(std::move(at));
		}
		else {
			std::unique_ptr<WindowFocusEvent> at = std::make_unique<WindowFocusEvent>(false);
			dispatcher->AddEvent(std::move(at));
		}

		int x, y;
		glfwGetWindowPos(window, &x, &y);
		if (x != winx) {
			winx = x;
			winy = y;
			std::unique_ptr<WindowMoveEvent> at = std::make_unique<WindowMoveEvent>((unsigned int)winx, (unsigned int)winy);
			dispatcher->AddEvent(std::move(at));
		} else if (y != winy) {
			winx = x;
			winy = y;
			std::unique_ptr<WindowMoveEvent> at = std::make_unique<WindowMoveEvent>((unsigned int)winx, (unsigned int)winy);
			dispatcher->AddEvent(std::move(at));
		}

		for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
			if (glfwGetKey(window, key) == GLFW_PRESS) {
				std::unique_ptr<KeyPressedEvent> at = std::make_unique<KeyPressedEvent>((KeyCode)key);
				dispatcher->AddEvent(std::move(at));
			}
			if (glfwGetKey(window, key) == GLFW_RELEASE) {
				std::unique_ptr<KeyReleasedEvent> at = std::make_unique<KeyReleasedEvent>((KeyCode)key);
				dispatcher->AddEvent(std::move(at));
			}
		}

		int blef = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		int bmid = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
		int brig = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		int blas = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LAST);
		int beig = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_8);
		int bsev = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_7);
		int bsix = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_6);
		int bfiv = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5);
		int bfor = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4);
		int btre = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3);
		int btwo = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
		int bone = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);

		if (blef == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>(((MouseCode)blef));
			dispatcher->AddEvent(std::move(at));
		}
		else if (blef == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)blef);
			dispatcher->AddEvent(std::move(at));
		}

		if (bmid == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)bmid);
			dispatcher->AddEvent(std::move(at));
		}
		else if (bmid == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)bmid);
			dispatcher->AddEvent(std::move(at));
		}

		if (brig == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)brig);
			dispatcher->AddEvent(std::move(at));
		}
		else if (brig == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)brig);
			dispatcher->AddEvent(std::move(at));
		}

		if (blas == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)blas);
			dispatcher->AddEvent(std::move(at));
		}
		else if (blas == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)blas);
			dispatcher->AddEvent(std::move(at));
		}

		if (beig == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)beig);
			dispatcher->AddEvent(std::move(at));
		}
		else if (beig == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)beig);
			dispatcher->AddEvent(std::move(at));
		}

		if (bsev == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)bsev);
			dispatcher->AddEvent(std::move(at));
		}
		else if (bsev == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)bsev);
			dispatcher->AddEvent(std::move(at));
		}

		if (bsix == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)bsix);
			dispatcher->AddEvent(std::move(at));
		}
		else if (bsix == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)bsix);
			dispatcher->AddEvent(std::move(at));
		}

		if (bfiv == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)bfiv);
			dispatcher->AddEvent(std::move(at));
		}
		else if (bfiv == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)bfiv);
			dispatcher->AddEvent(std::move(at));
		}

		if (bfor == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)bfor);
			dispatcher->AddEvent(std::move(at));
		}
		else if (bfor == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)bfor);
			dispatcher->AddEvent(std::move(at));
		}

		if (btre == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)btre);
			dispatcher->AddEvent(std::move(at));
		}
		else if (btre == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)btre);
			dispatcher->AddEvent(std::move(at));
		}

		if (btwo == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)btwo);
			dispatcher->AddEvent(std::move(at));
		}
		else if (btwo == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)btwo);
			dispatcher->AddEvent(std::move(at));
		}

		if (bone == GLFW_PRESS) {
			std::unique_ptr<MouseButtonPressedEvent> at = std::make_unique<MouseButtonPressedEvent>((MouseCode)bone);
			dispatcher->AddEvent(std::move(at));
		}
		else if (bone == GLFW_RELEASE) {
			std::unique_ptr<MouseButtonReleasedEvent> at = std::make_unique<MouseButtonReleasedEvent>((MouseCode)bone);
			dispatcher->AddEvent(std::move(at));
		}

		// Check mouse movement
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		if (mouseX != lastMouseX || mouseY != lastMouseY) {
			lastMouseX = mouseX;
			lastMouseY = mouseY;
			std::unique_ptr<MouseMovedEvent> at = std::make_unique<MouseMovedEvent>((float)lastMouseX, (float)lastMouseY);
			dispatcher->AddEvent(std::move(at));
		}

		if (a == true) {
			a = false;
			std::unique_ptr<MouseScrolledEvent> at = std::make_unique<MouseScrolledEvent>((float)xffset, (float)yffset);
			dispatcher->AddEvent(std::move(at));
		}

		std::vector<Object>* objes = std::any_cast<std::vector<Object>*>(rLayer);

		for (int i = 0; i < objes->size(); i++) {
			if (objes->at(i).modelMatrix == objes->at(i).OldmodelMatrix) {
				continue;
			}
			else {
				std::unique_ptr<ObjectMovedEvent> attt = std::make_unique<ObjectMovedEvent>(objes->at(i).modelMatrix, objes->at(i).OldmodelMatrix, objes->at(i).name);
				dispatcher->AddEvent(std::move(attt));
				objes->at(i).OldmodelMatrix = objes->at(i).modelMatrix;
			}
		}
	}

	void Window::Render() {

	}
}