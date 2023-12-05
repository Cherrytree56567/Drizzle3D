#include "Events.h"

namespace Drizzle3D {
    void Events::AddEventListener(EventType eventType, EventCallback callback) {
        eventCallbacks[eventType].emplace_back(std::move(callback));
    }

    void Events::RemoveEventListener(EventType eventType, EventCallback callback) {
        auto& callbacks = eventCallbacks[eventType];
        callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    }

    void Events::DispatchEvent(EventType eventType, HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) {
        auto& callbacks = eventCallbacks[eventType];
        for (const auto& callback : callbacks) {
            callback(hwnd, MSG, wparam, lparam);
        }
    }
}