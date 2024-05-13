#pragma once
#include <vector>
#include <string>
#include <memory>
#include "base.h"
#include "Events.h"
#include "App.h"
#include "ObjectWindow.h"

namespace Drizzle3D {
	struct AABB {
		glm::vec3 min;
		glm::vec3 max;
	};

	class Physics {
	public:
		Drizzle3D_API Physics(std::shared_ptr<App> ap);

		Drizzle3D_API void calculateAABB(std::string name);
		Drizzle3D_API void addCollider(std::string name, bool isCollider = true);
		Drizzle3D_API void UpdateColliders();
		Drizzle3D_API bool CheckColliders(std::string name);
	private:
		std::vector<std::tuple<std::string, bool, AABB>> colliders;
		std::shared_ptr<App> app;
	};
}