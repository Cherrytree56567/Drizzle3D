/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include "Events.h"

namespace Drizzle3D {
	class Drizzle3D_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		EventType GetEventType() override { return EventType::WindowResize; }
	private:
		unsigned int m_Width, m_Height;
	};

	class Drizzle3D_API WindowMoveEvent : public Event {
	public:
		WindowMoveEvent(unsigned int x, unsigned int y) : m_x(x), m_y(y) {}

		unsigned int GetWidth() const { return m_x; }
		unsigned int GetHeight() const { return m_y; }
		EventType GetEventType() override { return EventType::WindowMoved; }
	private:
		unsigned int m_x, m_y;
	};

	class Drizzle3D_API WindowFocusEvent : public Event {
	public:
		WindowFocusEvent(bool isFocused) : m_is_focused(isFocused) {}

		bool IsFocused() const { return m_is_focused; }
		EventType GetEventType() override { return EventType::WindowFocus; }
	private:
		bool m_is_focused;
	};

	class Drizzle3D_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;

		EventType GetEventType() override { return EventType::WindowClose; }
	};

	class Drizzle3D_API AppTickEvent : public Event {
	public:
		AppTickEvent() = default;

		EventType GetEventType() override { return EventType::AppTick; }
	};

	class Drizzle3D_API AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() = default;

		EventType GetEventType() override { return EventType::AppUpdate; }
	};

	class Drizzle3D_API AppRenderEvent : public Event {
	public:
		AppRenderEvent() = default;

		EventType GetEventType() override { return EventType::AppRender; }
	};
}