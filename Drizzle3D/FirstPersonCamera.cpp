/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "FirstPersonCamera.h"

namespace Drizzle3D {
	FirstPersonCamera::FirstPersonCamera(std::shared_ptr<App> app) : application(app) {
		// Initialize the camera
		// Switch to the FPS Camera
		// Make functions for returning the camera pos, up and look-at vectors
		// Use mouse callbacks to control the camera  

		/*double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, 1024 / 2, 768 / 2);

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
		verticalAngle += mouseSpeed * float(768 / 2 - ypos);
		*/
		// Direction : Spherical coordinates to Cartesian coordinates conversion
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(application->window()->returnwindow(), GLFW_KEY_UP) == GLFW_PRESS) {
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(application->window()->returnwindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(application->window()->returnwindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(application->window()->returnwindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}

		Camera cam{ {0,0,3}, {0,0,0}, {0,1,0}, (char*)"FirstPersonCamera"};

		application->GetRenderingLayer()->AddCamera("FirstPersonCamera", cam);
		application->GetRenderingLayer()->SwitchCamera("FirstPersonCamera");

		application->dispatcher()->AddEventListener(EventType::AppUpdate, [](GLFWwindow* window, std::unique_ptr<Drizzle3D::Event> ev, std::any a) {
			std::cout << "VENN: " << a.type().name();
			FirstPersonCamera* al = std::any_cast<FirstPersonCamera*>(a);
			std::cout << al << std::endl;

			double xpos = al->xpos;
			double ypos = al->ypos;
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			if (al->capture) {
				glfwGetCursorPos(window, &xpos, &ypos);
				al->xpos = xpos;
				al->ypos = ypos;

				// Reset mouse position for next frame
				glfwSetCursorPos(window, width / 2, height / 2);
			}
			// Compute new orientation
			al->horizontalAngle += al->mouseSpeed * float(width / 2 - xpos);
			al->verticalAngle += al->mouseSpeed * float(height / 2 - ypos);
			
			glm::vec3 direction(
				cos(al->verticalAngle) * sin(al->horizontalAngle),
				sin(al->verticalAngle),
				cos(al->verticalAngle) * cos(al->horizontalAngle)
			);

			// Right vector
			glm::vec3 right = glm::vec3(
				sin(al->horizontalAngle - 3.14f / 2.0f),
				0,
				cos(al->horizontalAngle - 3.14f / 2.0f)
			);

			// Up vector
			glm::vec3 up = glm::cross(right, direction);

			// Move forward
			if (glfwGetKey(al->application->window()->returnwindow(), GLFW_KEY_UP) == GLFW_PRESS) {
				al->position += direction * al->speed;
			}
			// Move backward
			if (glfwGetKey(al->application->window()->returnwindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
				al->position -= direction * al->speed;
			}
			// Strafe right
			if (glfwGetKey(al->application->window()->returnwindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
				al->position += right * al->speed;
			}
			// Strafe left
			if (glfwGetKey(al->application->window()->returnwindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
				al->position -= right * al->speed;
			}

			if (glfwGetKey(al->application->window()->returnwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				al->capture = false;
			}

			al->application->GetRenderingLayer()->returnCamera("FirstPersonCamera")->position = al->position;
			al->application->GetRenderingLayer()->returnCamera("FirstPersonCamera")->up = up;
			al->application->GetRenderingLayer()->returnCamera("FirstPersonCamera")->look_at_position = al->position + direction;
		}, this);
	}


}