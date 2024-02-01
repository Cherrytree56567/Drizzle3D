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
	class Drizzle3D_API Skybox {
	public:
		Skybox(App& app, const char* skyboxtex, float size = 100.0f);
		void Update();

	private:
		App& application;
		glm::vec3 pos;
	};
}