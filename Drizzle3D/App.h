/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include "Window.h"
#include "ImGuiLayer.h"
#include "RenderingLayer.h"
#include "layer.h"
#include "logging.h"
#include "AppEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "ResourceManager.h"

#define AddEventCallback dispatcher()->AddEventListener
#define EWindowClose Drizzle3D::EventType::WindowClose
#define EWindowResize Drizzle3D::EventType::WindowResize
#define EWindowFocus Drizzle3D::EventType::WindowFocus
#define EWindowLostFocus Drizzle3D::EventType::WindowLostFocus
#define EWindowMoved Drizzle3D::EventType::WindowMoved
#define EAppTick Drizzle3D::EventType::AppTick
#define EAppUpdate Drizzle3D::EventType::AppUpdate
#define EAppRender Drizzle3D::EventType::AppRender
#define EKeyPressed Drizzle3D::EventType::KeyPressed
#define EKeyReleased Drizzle3D::EventType::KeyReleased
#define EMouseLeftButtonPressed Drizzle3D::EventType::MouseLeftButtonPressed
#define EMouseLeftButtonReleased Drizzle3D::EventType::MouseLeftButtonReleased
#define EMouseRightButtonPressed Drizzle3D::EventType::MouseRightButtonPressed
#define EMouseRightButtonReleased Drizzle3D::EventType::MouseRightButtonReleased
#define EMouseMoved Drizzle3D::EventType::MouseMoved

namespace Drizzle3D {
	class Drizzle3D_API App {
	public:
		App(char* WindowName = (char*)"New Drizzle3D Game", int width = 800, int height = 600);

		void Run();

		Window* window() { return &D3DWindow; }
		std::shared_ptr<ImGuiLayer> ImguiLayer() { return imguilayer; }
		std::shared_ptr<RenderingLayer> GetRenderingLayer() { return renderinglayer; }
		EventDispatcher* dispatcher() { return &dispatch; }

		typedef void(*UpdateFunc)(App* myApp);
		UpdateFunc update = [](App* myApp){};

	private:
		Window D3DWindow;

		// Layers
		std::shared_ptr<ImGuiLayer> imguilayer;
		std::shared_ptr<RenderingLayer> renderinglayer;

		// Dispatchers
		EventDispatcher dispatch;
		LayerDispatch LayerDispatcher;

		// Managers
		ResourceManager resourcemgr;
	};
}