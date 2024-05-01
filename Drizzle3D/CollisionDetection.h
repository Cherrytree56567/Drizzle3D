#pragma once
#include <vector>
#include <string>
#include "base.h"

namespace Drizzle3D {
	class CollisionDetection {
	public:
		CollisionDetection(std::vector<std::tuple<std::string, std::vector<float>, std::vector<unsigned int>>> col) : colliders(col) {}
		std::vector<std::string> CheckCollision();

	private:
		std::vector<std::tuple<std::string, std::vector<float>, std::vector<unsigned int>>> colliders;
	};
}