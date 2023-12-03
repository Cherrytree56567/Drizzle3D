#pragma once
#include <vector>
#include <GLFW/glfw3.h>

namespace Drizzle3D {
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	class EventDispatcher {
	public:
		EventDispatcher() {};
		typedef void (*Function)(GLFWwindow* window);
		typedef void (*FunctionA)(GLFWwindow* window, int width, int height);
		Function WindowClose;
		FunctionA WindowResize;
		Function WindowFocus;
		Function WindowLostFocus;
		FunctionA WindowMoved;
		Function AppTick;
		Function AppUpdate;
		Function AppRender;
		Function KeyPressed;
		Function KeyReleased;
		Function MouseButtonPressed;
		Function MouseButtonReleased;
		Function MouseMoved;
		Function MouseScrolled;
		void OnWindowClose(GLFWwindow* window) { WindowClose(window); }
		void OnWindowResize(GLFWwindow* window, int width, int height) { WindowResize(window, width, height); }
		void OnWindowFocus(GLFWwindow* window) { WindowFocus(window); }
		void OnWindowLostFocus(GLFWwindow* window) { WindowLostFocus(window); }
		void OnWindowMoved(GLFWwindow* window, int xpos, int ypos) { WindowMoved(window, xpos, ypos); }
		void OnAppTick(GLFWwindow* window) { AppTick(window); }
		void OnAppUpdate(GLFWwindow* window) { AppUpdate(window); }
		void OnAppRender(GLFWwindow* window) { AppRender(window); }
		void OnKeyPressed(GLFWwindow* window) { KeyPressed(window); }
		void OnKeyReleased(GLFWwindow* window) { KeyReleased(window); }
		void OnMouseButtonPressed(GLFWwindow* window) { MouseButtonPressed(window); }
		void OnMouseButtonReleased(GLFWwindow* window) { MouseButtonReleased(window); }
		void OnMouseMoved(GLFWwindow* window) { MouseMoved(window); }
		void OnMouseScrolled(GLFWwindow* window) { MouseScrolled(window); }
	};
}