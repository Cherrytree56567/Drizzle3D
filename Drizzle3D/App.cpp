/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "App.h"

namespace Drizzle3D {
	App::App(char* WindowName, int width, int height) : D3DWindow(&dispatch, WindowName, width, height), imguilayer(std::make_shared<ImGuiLayer>(&D3DWindow)), renderlayer(std::make_shared<RenderingLayer>(&D3DWindow)) {
		imguilayer->setIGUI(imguilayer);
		LayerDispatcher.AddLayer(renderlayer);
		LayerDispatcher.AddLayer(imguilayer);
		LayerDispatcher.DispatchLayerAttach();
	}

	void App::Run() {
		while (!glfwWindowShouldClose(D3DWindow.returnwindow())) {
			dispatch.ResetEvents();
			std::unique_ptr<AppTickEvent> a = std::make_unique<AppTickEvent>();
			dispatch.AddEvent(std::move(a));
			D3DWindow.ProcessEvents();
			std::unique_ptr<AppUpdateEvent> b = std::make_unique<AppUpdateEvent>();
			dispatch.AddEvent(std::move(b));
			update(this);
			dispatch.DispatchEvent(D3DWindow.returnwindow());
			std::unique_ptr<AppRenderEvent> r = std::make_unique<AppRenderEvent>();
			dispatch.AddEvent(std::move(r));
			D3DWindow.Render();
			LayerDispatcher.DispatchLayerRender();
			D3DWindow.clearKeyCodes();
			D3DWindow.clearKeyReleasedCodes();
			dispatch.ResetEvents();
		}
	}
}