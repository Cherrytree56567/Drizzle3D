#include "Application.h"

namespace Drizzle3D {
	void Application::Run() {
		while (D3DWindow.Update()) {
			Update(this);
			Render();
		}
	}
	void Application::Render(){
		D3DWindow.GetRenderer().RenderFrame();
	}
}