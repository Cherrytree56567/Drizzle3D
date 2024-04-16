/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "Skybox.h"

namespace Drizzle3D {
	Skybox::Skybox(std::shared_ptr<App> app, const char* skyboxtex, float size) : application(app) {
		switch (app->getRenderingAPI()) {
		case RenderingAPI::OpenGL:
			glDisable(GL_CULL_FACE);
			break;

		case RenderingAPI::Vulkan:
			std::cout << "[Drizzle3D::Core::Skybox] Warning: Vulkan Support is experimental";
		}
		glm::mat4 model = glm::mat4(1.0f);
		float d = size;

		model[0][0] = size; // Scaling along X-axis
		model[1][1] = size; // Scaling along Y-axis
		model[2][2] = size; // Scaling along Z-axis
		application->GetRenderingLayer()->AddObject("Skybox", application->GetRenderingLayer()->DrawVerts(LoadObjFile("Skybox.obj"), model));
		application->GetRenderingLayer()->returnObject("Skybox")->textureID = app->GetRenderingLayer()->GetTexture(skyboxtex);

		application->dispatcher()->AddEventListener(EventType::AppUpdate, [](GLFWwindow* window, std::unique_ptr<Drizzle3D::Event> ev, std::any a) {
			Skybox* al = std::any_cast<Skybox*>(a);

			al->Update();
		}, this);
	}

	void Skybox::Update() {
		// Get the camera position
		glm::vec3 cameraPosition = application->GetRenderingLayer()->ReturnActiveCamera().position;
		std::cout << "X: " << cameraPosition.x << " Y: " << cameraPosition.y << " Z: " << cameraPosition.z << "\n";

		pos = cameraPosition;

		application->GetRenderingLayer()->returnObject("Skybox")->modelMatrix[3][0] = pos.x; // Translation along X-axis
		application->GetRenderingLayer()->returnObject("Skybox")->modelMatrix[3][1] = pos.y; // Translation along Y-axis
		application->GetRenderingLayer()->returnObject("Skybox")->modelMatrix[3][2] = pos.z; // Translation along Z-axis

	}
}