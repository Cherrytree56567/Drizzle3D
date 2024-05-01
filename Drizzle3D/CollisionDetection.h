#pragma once
#include <vector>
#include <string>
#include "base.h"
#include "Events.h"

namespace Drizzle3D {
	class CollisionDetection {
	public:
		Drizzle3D_API CollisionDetection(std::vector<std::tuple<std::string, std::vector<float>, std::vector<unsigned int>>> col, std::shared_ptr<EventDispatcher> disp) : colliders(col), dispatcher(disp) {}
		Drizzle3D_API void CheckCollision();

	private:
		std::vector<std::tuple<std::string, std::vector<float>, std::vector<unsigned int>>> colliders;
		std::shared_ptr<EventDispatcher> dispatcher;
	};
}