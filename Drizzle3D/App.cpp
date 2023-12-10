#include "App.h"

namespace Drizzle3D {
	App::App(char* WindowName, int width, int height) : D3DWindow(&dispatch, WindowName, width, height), imguilayer(&D3DWindow), renderinglayer(&D3DWindow) {
		LayerDispatcher.AddLayer(&renderinglayer);
		LayerDispatcher.AddLayer(&imguilayer);
		LayerDispatcher.DispatchLayerAttach();
	}

	void App::Run() {
		while (!glfwWindowShouldClose(D3DWindow.returnwindow())) {
			D3DWindow.ProcessEvents();
			update(this);
			D3DWindow.Render();
			LayerDispatcher.DispatchLayerRender();
			D3DWindow.clearKeyCodes();
			D3DWindow.clearKeyReleasedCodes();
		}
	}
}