/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include "Events.h"
#include "MouseCodes.h"

namespace Drizzle3D {
	class Drizzle3D_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(const float x, const float y) : m_MouseX(x), m_MouseY(y) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		EventType GetEventType() override { return EventType::MouseMoved; }
	private:
		float m_MouseX, m_MouseY;
	};

	class Drizzle3D_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		EventType GetEventType() override { return EventType::MouseScrolled; }
	private:
		float m_XOffset, m_YOffset;
	};

	class Drizzle3D_API MouseButtonPressedEvent : public Event {
	public:
		MouseButtonPressedEvent(const MouseCode button) : m_Button(button) {}

		MouseCode GetMouseButton() const { return m_Button; }

		EventType GetEventType() override { return EventType::MouseButtonPressed; }
	private:
		MouseCode m_Button;
	};

	class Drizzle3D_API MouseButtonReleasedEvent : public Event {
	public:
		MouseButtonReleasedEvent(const MouseCode button) : m_Button(button) {}

		MouseCode GetMouseButton() const { return m_Button; }

		EventType GetEventType() override { return EventType::MouseButtonReleased; }
	private:
		MouseCode m_Button;
	};
}