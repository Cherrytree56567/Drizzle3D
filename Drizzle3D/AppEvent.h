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
	class WindowResizeEvent : public Event {
	public:
		Drizzle3D_API WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

		Drizzle3D_API unsigned int GetWidth() const { return m_Width; }
		Drizzle3D_API unsigned int GetHeight() const { return m_Height; }

		Drizzle3D_API EventType GetEventType() override { return EventType::WindowResize; }
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowMoveEvent : public Event {
	public:
		Drizzle3D_API WindowMoveEvent(unsigned int x, unsigned int y) : m_x(x), m_y(y) {}

		Drizzle3D_API unsigned int GetWidth() const { return m_x; }
		Drizzle3D_API unsigned int GetHeight() const { return m_y; }
		Drizzle3D_API EventType GetEventType() override { return EventType::WindowMoved; }
	private:
		unsigned int m_x, m_y;
	};

	class WindowFocusEvent : public Event {
	public:
		Drizzle3D_API WindowFocusEvent(bool isFocused) : m_is_focused(isFocused) {}

		Drizzle3D_API bool IsFocused() const { return m_is_focused; }
		Drizzle3D_API EventType GetEventType() override { return EventType::WindowFocus; }
	private:
		bool m_is_focused;
	};

	class WindowCloseEvent : public Event {
	public:
		Drizzle3D_API WindowCloseEvent() = default;

		Drizzle3D_API EventType GetEventType() override { return EventType::WindowClose; }
	};

	class AppTickEvent : public Event {
	public:
		Drizzle3D_API AppTickEvent() = default;

		Drizzle3D_API EventType GetEventType() override { return EventType::AppTick; }
	};

	class AppUpdateEvent : public Event {
	public:
		Drizzle3D_API AppUpdateEvent() = default;

		Drizzle3D_API EventType GetEventType() override { return EventType::AppUpdate; }
	};

	class AppRenderEvent : public Event {
	public:
		Drizzle3D_API AppRenderEvent() = default;

		Drizzle3D_API EventType GetEventType() override { return EventType::AppRender; }
	};

	class ObjectColliderEvent : public Event {
	public:
		Drizzle3D_API ObjectColliderEvent(std::vector<std::string> col) : collided(col) {};

		Drizzle3D_API std::vector<std::string> GetColliders() { return collided; }

		Drizzle3D_API EventType GetEventType() override { return EventType::Collided; }
	private:
		std::vector<std::string> collided;
	};
}