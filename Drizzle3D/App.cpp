#include "App.h"

namespace Drizzle3D {
	App::App(char* WindowName, int width, int height) : D3DWindow(&dispatch, WindowName, width, height), imguilayer(&D3DWindow) {
		LayerDispatcher.AddLayer(&imguilayer);
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