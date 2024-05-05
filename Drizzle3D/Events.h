/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include <functional>
#include <any>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include "Logging.h"

namespace Drizzle3D {
    enum EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        ObjectMoved
    };

    class Event {
    public:
        Drizzle3D_API virtual ~Event() = default;

        bool Handled = false;

        Drizzle3D_API virtual EventType GetEventType() = 0;
    };

    class EventDispatcher {
    public:
        Drizzle3D_API EventDispatcher(const EventDispatcher&) = delete;
        Drizzle3D_API EventDispatcher& operator=(const EventDispatcher&) = delete;
        Drizzle3D_API EventDispatcher() = default;
        Drizzle3D_API EventDispatcher& operator=(EventDispatcher&&) = default;
        Drizzle3D_API EventDispatcher(EventDispatcher&&) = default;
        typedef void (*EventCallback)(GLFWwindow* app, std::unique_ptr<Event> events, std::any);
        Drizzle3D_API void AddEvent(std::unique_ptr<Event> newEvent);
        Drizzle3D_API std::unique_ptr<Event> GetEvent(EventType eventType);
        Drizzle3D_API void RemoveHandledEvents();
        Drizzle3D_API bool isUnhandledEvent(EventType eventType);
        Drizzle3D_API void ResetEvents() { events.clear(); }
        Drizzle3D_API void AddEventListener(EventType eventType, EventCallback callback, std::any a = NULL);
        Drizzle3D_API void RemoveEventListener(EventType eventType, EventCallback callback);
        Drizzle3D_API void DispatchEvent(GLFWwindow* window);
    private:
        std::vector<std::unique_ptr<Event>> events;
        std::unordered_map<EventType, std::vector<std::pair<EventCallback, std::any>>> eventCallbacks;
        Logging log;
    };
}
