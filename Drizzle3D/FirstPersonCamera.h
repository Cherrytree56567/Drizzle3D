/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Events.h"
#include "App.h"

namespace Drizzle3D {
	class Drizzle3D_API FirstPersonCamera {
	public:
		FirstPersonCamera(App& app);
		void Update();

		glm::vec3 position = glm::vec3(0, 0, 5);
		float horizontalAngle = 0.0f;
		float verticalAngle = 0.0f;
		float initialFoV = 450.0f;

		float speed = 1.0f; // 3 units / second
		float mouseSpeed = 0.005f;
		App& application;
		bool capture = true;
		double xpos, ypos = 0.0;
	};
}