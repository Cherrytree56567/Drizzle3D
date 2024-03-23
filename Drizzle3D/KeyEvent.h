/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include "Events.h"
#include "KeyCodes.h"

namespace Drizzle3D {

	class KeyPressedEvent : public Event {
	public:
		Drizzle3D_API KeyPressedEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

		Drizzle3D_API KeyCode GetKeyCode() { return m_KeyCode; }

		Drizzle3D_API EventType GetEventType() override { return EventType::KeyPressed; }
	private:
		KeyCode m_KeyCode;
	};

	class KeyReleasedEvent : public Event {
	public:
		Drizzle3D_API KeyReleasedEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

		Drizzle3D_API KeyCode GetKeyCode() { return m_KeyCode; }

		Drizzle3D_API EventType GetEventType() override { return EventType::KeyReleased; }
	private:
		KeyCode m_KeyCode;
	};
}