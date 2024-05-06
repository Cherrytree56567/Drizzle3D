#pragma once
#include <vector>
#include <string>
#include "base.h"
#include "Events.h"
#include "App.h"
#include "ObjectWindow.h"

namespace Drizzle3D {
	class Physics {
	public:
		Drizzle3D_API Physics(std::shared_ptr<App> ap) : app(ap) {}
		Drizzle3D_API void CheckCollision();
		Drizzle3D_API void AddCollider(const char* name, bool isCollider, std::vector<float> verts = std::vector<float>(NULL), std::vector<unsigned int> indecies = std::vector<unsigned int>(NULL));
		Drizzle3D_API void RemoveCollider(std::string name);

	private:
		std::vector<std::tuple<std::string, bool, std::vector<float>, std::vector<unsigned int>>> colliders;
		std::shared_ptr<App> app;
	};
}