#include "App.h"

namespace Drizzle3D {
	App::App(char* WindowName, int width, int height) : D3DWindow(&dispatch, WindowName, width, height), imguilayer(&D3DWindow), renderinglayer(&D3DWindow) {
		LayerDispatcher.AddLayer(&imguilayer);
		LayerDispatcher.AddLayer(&renderinglayer);
		LayerDispatcher.ChangeLayerOrderByName("ImGUI", 1);
		LayerDispatcher.DispatchLayerAttach();
	}

	void App::Run() {
		while (!glfwWindowShouldClose(D3DWindow.returnwindow())) {
			D3DWindow.ProcessEvents();
			dispatch.DispatchEvent(EventType::AppUpdate, D3DWindow.returnwindow());
			D3DWindow.Render();
			LayerDispatcher.DispatchLayerRender();
			dispatch.DispatchEvent(EventType::AppTick, D3DWindow.returnwindow());
			D3DWindow.clearKeyCodes();
			D3DWindow.clearKeyReleasedCodes();
		}
	}
}