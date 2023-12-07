#pragma once
#include "Window.h"
#include "ImGuiLayer.h"
#include "RenderingLayer.h"
#include "layer.h"

namespace Drizzle3D {
	class App {
	public:
		App(char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);

		void Run();

		Window* window() { return &D3DWindow; }
		ImGuiLayer* ImguiLayer() { return &imguilayer; }
		RenderingLayer* GetRenderingLayer() { return &renderinglayer; }
		Events* dispatcher() { return &dispatch; }

	private:
		Window D3DWindow;

		// Layers
		ImGuiLayer imguilayer;
		RenderingLayer renderinglayer;

		// Dispatchers
		Events dispatch;
		LayerDispatch LayerDispatcher;
	};
}