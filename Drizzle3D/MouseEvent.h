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
	class MouseMovedEvent : public Event {
	public:
		Drizzle3D_API MouseMovedEvent(const float x, const float y) : m_MouseX(x), m_MouseY(y) {}

		Drizzle3D_API float GetX() const { return m_MouseX; }
		Drizzle3D_API float GetY() const { return m_MouseY; }

		Drizzle3D_API EventType GetEventType() override { return EventType::MouseMoved; }
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event {
	public:
		Drizzle3D_API MouseScrolledEvent(const float xOffset, const float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

		Drizzle3D_API float GetXOffset() const { return m_XOffset; }
		Drizzle3D_API float GetYOffset() const { return m_YOffset; }

		Drizzle3D_API EventType GetEventType() override { return EventType::MouseScrolled; }
	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonPressedEvent : public Event {
	public:
		Drizzle3D_API MouseButtonPressedEvent(const MouseCode button) : m_Button(button) {}

		Drizzle3D_API MouseCode GetMouseButton() const { return m_Button; }

		Drizzle3D_API EventType GetEventType() override { return EventType::MouseButtonPressed; }
	private:
		MouseCode m_Button;
	};

	class MouseButtonReleasedEvent : public Event {
	public:
		Drizzle3D_API MouseButtonReleasedEvent(const MouseCode button) : m_Button(button) {}

		Drizzle3D_API MouseCode GetMouseButton() const { return m_Button; }

		Drizzle3D_API EventType GetEventType() override { return EventType::MouseButtonReleased; }
	private:
		MouseCode m_Button;
	};
}