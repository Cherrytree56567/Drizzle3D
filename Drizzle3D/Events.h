/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <glad/glad.h>
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
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    class Drizzle3D_API Event {
    public:
        virtual ~Event() = default;

        bool Handled = false;

        virtual EventType GetEventType() = 0;
    };

    class Drizzle3D_API EventDispatcher {
    public:
        EventDispatcher(const EventDispatcher&) = delete;
        EventDispatcher& operator=(const EventDispatcher&) = delete;
        EventDispatcher() = default;
        EventDispatcher& operator=(EventDispatcher&&) = default;
        EventDispatcher(EventDispatcher&&) = default;
        typedef void (*EventCallback)(GLFWwindow* app, std::unique_ptr<Event> events, std::any);
        void AddEvent(std::unique_ptr<Event> newEvent);
        std::unique_ptr<Event> GetEvent(EventType eventType);
        void RemoveHandledEvents();
        bool isUnhandledEvent(EventType eventType);
        void ResetEvents() { events.clear(); }
        void AddEventListener(EventType eventType, EventCallback callback, std::any a = NULL);
        void RemoveEventListener(EventType eventType, EventCallback callback);
        void DispatchEvent(GLFWwindow* window);
    private:
        std::vector<std::unique_ptr<Event>> events;
        std::unordered_map<EventType, std::vector<std::pair<EventCallback, std::any>>> eventCallbacks;
        Logging log;
    };
}
