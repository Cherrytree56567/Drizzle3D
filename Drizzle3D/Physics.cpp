#include "Physics.h"

namespace Drizzle3D {
	void Physics::CheckCollision() {
		// Create Collision Event
		app->dispatcher()->AddEventListener(Drizzle3D::EventType::ObjectMoved, [](GLFWwindow* window, std::unique_ptr<Drizzle3D::Event> ev, std::any a) {
			Physics* al = std::any_cast<Physics*>(a);
		}, this);
	}

	void Physics::AddCollider(const char* name, bool isCollider, std::vector<float> verts, std::vector<unsigned int> indecies) {
		if (verts == std::vector<float>(NULL) && indecies == std::vector<unsigned int>(NULL)) {
			std::vector<float> vertecies;
			int i = 0;
			while (true) {
				if (i > app->GetRenderingLayer()->returnObject(name)->vertices.size()) {
					break;
				}

				vertecies.push_back(app->GetRenderingLayer()->returnObject(name)->vertices[i]);
				i++;
				vertecies.push_back(app->GetRenderingLayer()->returnObject(name)->vertices[i]);
				i++;
				vertecies.push_back(app->GetRenderingLayer()->returnObject(name)->vertices[i]);
				i += 7;
			}
			colliders.push_back(std::make_tuple(name, isCollider, vertecies, indecies));
		}
		else {
			colliders.push_back(std::make_tuple(name, isCollider, verts, indecies));
		}
	}

	void Physics::RemoveCollider(std::string name) {
		auto it = std::remove_if(colliders.begin(), colliders.end(), [&](const std::tuple<std::string, bool, std::vector<float>, std::vector<unsigned int>>& element) {
			return get<0>(element) == name;
			});

		// Erase the removed elements
		colliders.erase(it, colliders.end());
	}
}