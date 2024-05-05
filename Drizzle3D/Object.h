#pragma once
#include "Events.h"
#include <glm/glm.hpp>

namespace Drizzle3D {
	class ObjectMovedEvent : public Event {
	public:
		Drizzle3D_API ObjectMovedEvent(glm::mat4 mM, glm::mat4 OldmM) : modelMatrix(mM), OldmodelMatrix(OldmM) {};

		Drizzle3D_API glm::mat4 GetModelMatrix() { return modelMatrix; }
		Drizzle3D_API glm::mat4 GetOldModelMatrix() { return OldmodelMatrix; }

		Drizzle3D_API EventType GetEventType() override { return EventType::ObjectMoved; }
	private:
		glm::mat4 modelMatrix;
		glm::mat4 OldmodelMatrix;
	};
}