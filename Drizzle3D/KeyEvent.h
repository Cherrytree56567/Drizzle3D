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

	class Drizzle3D_API KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

		KeyCode GetKeyCode() { return m_KeyCode; }

		EventType GetEventType() override { return EventType::KeyPressed; }
	private:
		KeyCode m_KeyCode;
	};

	class Drizzle3D_API KeyReleasedEvent : public Event {
	public:
		KeyReleasedEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

		KeyCode GetKeyCode() { return m_KeyCode; }

		EventType GetEventType() override { return EventType::KeyReleased; }
	private:
		KeyCode m_KeyCode;
	};
}