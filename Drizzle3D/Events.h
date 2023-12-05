#pragma once
#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <Windows.h>

namespace Drizzle3D {
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseLeftButtonPressed, MouseLeftButtonReleased, MouseRightButtonPressed, MouseRightButtonReleased, MouseMoved, MouseScrolled
    };

    class Events
    {
    public:
        typedef void (*EventCallback)(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam);

        void AddEventListener(EventType eventType, EventCallback callback);

        void RemoveEventListener(EventType eventType, EventCallback callback);

        void DispatchEvent(EventType eventType, HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam);

    private:
        std::unordered_map<EventType, std::vector<EventCallback>> eventCallbacks;
    };
}