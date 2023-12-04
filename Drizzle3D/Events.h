#pragma once
#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Drizzle3D {
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    class Events
    {
    public:
        typedef void (*EventCallback)();

        void AddEventListener(EventType eventType, EventCallback callback);

        void RemoveEventListener(EventType eventType, EventCallback callback);

        void DispatchEvent(EventType eventType);

    private:
        std::unordered_map<EventType, std::vector<EventCallback>> eventCallbacks;
    };
}