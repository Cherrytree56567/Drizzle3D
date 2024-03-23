/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include "App.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Drizzle3D {
	class Skybox {
	public:
		Drizzle3D_API Skybox(std::shared_ptr<App> app, const char* skyboxtex, float size = 100.0f);
		Drizzle3D_API void Update();

	private:
		std::shared_ptr<App> application;
		glm::vec3 pos;
	};
}