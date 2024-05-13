#include "Physics.h"

namespace Drizzle3D {

	Physics::Physics(std::shared_ptr<App> ap) : app(ap) {
		app->dispatcher()->AddEventListener(Drizzle3D::EventType::ObjectMoved, [](GLFWwindow* window, std::unique_ptr<Drizzle3D::Event> ev, std::any a) {
			Physics* al = std::any_cast<Physics*>(a);
			al->calculateAABB(dynamic_cast<Drizzle3D::ObjectMovedEvent*>(ev.get())->GetObjectName());

			if (al->CheckColliders(dynamic_cast<Drizzle3D::ObjectMovedEvent*>(ev.get())->GetObjectName())) {
				std::cout << "Object Collided!";
				exit(-1993);
			}
		}, this);
	}

	void Physics::calculateAABB(std::string name) {
		if (name == "Skybox")
			return;

		glm::vec3 min_bound = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 max_bound = glm::vec3(std::numeric_limits<float>::lowest());

		// Assuming vertices are stored as x, y, z, x, y, z, ...
		auto Objects = app->GetRenderingLayer()->getObjects();
		std::vector<float> vertices;

		for (int i = 0; i < Objects->size(); i++) {
			if (Objects->at(i).name == name) {
				vertices = Objects->at(i).vertices;
			}
		}

		for (size_t i = 0; i < vertices.size(); i += 3) {
			glm::vec3 vertex(vertices[i], vertices[i + 1], vertices[i + 2]);
			min_bound = glm::min(min_bound, vertex);
			max_bound = glm::max(max_bound, vertex);
		}

		// Update collider's AABB if it exists for the specified object
		for (auto& collider : colliders) {
			if (std::get<0>(collider) == name) {
				std::get<2>(collider).min = min_bound;
				std::get<2>(collider).max = max_bound;
				break;
			}
		}
	}

	void Physics::addCollider(std::string name, bool isCollider) {
		for (auto& collider : colliders) {
			if (std::get<0>(collider) == name) {
				std::get<1>(collider) = isCollider;
				break;
			}
		}
	}

	void Physics::UpdateColliders() {
		for (auto& obj : *app->GetRenderingLayer()->getObjects()) {
			bool found = false;
			for (auto& collider : colliders) {
				if (std::get<0>(collider) == obj.name) {
					found = true;
					break;
				}
			}
			if (!found && obj.name != "Skybox") {
				colliders.push_back(std::make_tuple(obj.name, false, AABB(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0))));
				calculateAABB(obj.name);
			}
		}
	}

	bool Physics::CheckColliders(std::string name) {
		glm::vec3 objectMin;
		glm::vec3 objectMax;
		bool isCollider = false;
		for (const auto& collider : colliders) {
			if (std::get<0>(collider) == name) {
				objectMin = std::get<2>(collider).min;
				objectMax = std::get<2>(collider).max;
				isCollider = std::get<1>(collider);
				break;
			}
		}

		if (!isCollider)
			return false;

		for (const auto& otherCollider : colliders) {
			if (std::get<0>(otherCollider) == name || !std::get<1>(otherCollider))
				continue;

			glm::vec3 otherMin = std::get<2>(otherCollider).min;
			glm::vec3 otherMax = std::get<2>(otherCollider).max;

			if (objectMin.x <= otherMax.x && objectMax.x >= otherMin.x &&
				objectMin.y <= otherMax.y && objectMax.y >= otherMin.y &&
				objectMin.z <= otherMax.z && objectMax.z >= otherMin.z) {
				return true;
			}
		}

		return false;
	}
}