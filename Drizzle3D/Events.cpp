#include "Events.h"

namespace Drizzle3D {
    void Events::AddEventListener(EventType eventType, EventCallback callback) {
        eventCallbacks[eventType].emplace_back(std::move(callback));
    }

    void Events::RemoveEventListener(EventType eventType, EventCallback callback) {
        auto& callbacks = eventCallbacks[eventType];
        callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    }

    void Events::DispatchEvent(EventType eventType, GLFWwindow* window) {
        auto& callbacks = eventCallbacks[eventType];
        for (const auto& callback : callbacks) {
            callback(window);
        }
        switch (eventType) {
        case Drizzle3D::EventType::WindowClose:
            log.Info("Window Closed.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::WindowResize:
            log.Info("Window Resized.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::WindowFocus:
            log.Info("Window is Focused.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::WindowLostFocus:
            log.Info("Window has Lost Focus.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::WindowMoved:
            log.Info("Window Moved.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::AppTick:
            log.Info("Application Tick.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::AppUpdate:
            log.Info("Application Update.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::AppRender:
            log.Info("Application Render.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::KeyPressed:
            log.Info("Key Pressed.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::MouseLeftButtonPressed:
            log.Info("Mouse Button Pressed.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::MouseLeftButtonReleased:
            log.Info("Mouse Button Released.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::MouseRightButtonPressed:
            log.Info("Mouse Button Pressed.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::MouseRightButtonReleased:
            log.Info("Mouse Button Released.", "[Drizzle3D::Core::Events]");
            break;
        case Drizzle3D::EventType::MouseMoved:
            log.Info("Mouse Moved.", "[Drizzle3D::Core::Events]");
            break;
        default:
            break;
        }
    }
}