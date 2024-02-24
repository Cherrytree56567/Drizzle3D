/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <vector>
#include <iostream>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "AppEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "base.h"
#include <d3d11.h>

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
		ID3D11Device* returnDX11Device() { return device; }
		ID3D11DeviceContext* returnDX11DeviceContext() { return deviceContext; }
		IDXGISwapChain* returnDXGISwapChain() { return swapChain; }
		ID3D11RenderTargetView* returnD3D11RenderTargetView() { return swapChainRenderTargetView; }

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
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		IDXGISwapChain* swapChain;
		ID3D11RenderTargetView* swapChainRenderTargetView;
	};

}