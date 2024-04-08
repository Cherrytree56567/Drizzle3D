/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "App.h"

namespace Drizzle3D {
	App::App(RenderingAPI rAPI, char* WindowName, int width, int height) : renderingAPI(rAPI), D3DWindow(&dispatch, WindowName, width, height), imguilayer(std::make_shared<ImGuiLayer>(rAPI, &D3DWindow)), resourcemgr(std::make_shared<ResourceManager>()), renderinglayer(std::make_shared<RenderingLayer>(rAPI, &D3DWindow, resourcemgr)), renderinglayer2d(std::make_shared<RenderingLayer2D>(rAPI, &D3DWindow, resourcemgr)) {
		imguilayer->setIGUI(imguilayer);
		LayerDispatcher.AddLayer(renderinglayer2d);
		LayerDispatcher.AddLayer(renderinglayer);
		LayerDispatcher.AddLayer(imguilayer);
		LayerDispatcher.DispatchLayerAttach();
	}

	bool App::Run() {
		dispatch.ResetEvents();
		std::unique_ptr<AppTickEvent> a = std::make_unique<AppTickEvent>();
		dispatch.AddEvent(std::move(a));
		D3DWindow.ProcessEvents();
		std::unique_ptr<AppUpdateEvent> b = std::make_unique<AppUpdateEvent>();
		dispatch.AddEvent(std::move(b));
		dispatch.DispatchEvent(D3DWindow.returnwindow());
		std::unique_ptr<AppRenderEvent> r = std::make_unique<AppRenderEvent>();
		dispatch.AddEvent(std::move(r));
		D3DWindow.Render();
		LayerDispatcher.DispatchLayerRender();
		D3DWindow.clearKeyCodes();
		D3DWindow.clearKeyReleasedCodes();
		dispatch.ResetEvents();
		return !glfwWindowShouldClose(D3DWindow.returnwindow());
	}
}