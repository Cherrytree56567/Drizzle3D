/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "Events.h"

namespace Drizzle3D {
    void EventDispatcher::AddEvent(std::unique_ptr<Event> newEvent) {
        events.push_back(std::move(newEvent));
    }

    std::unique_ptr<Event> EventDispatcher::GetEvent(EventType eventType) {
        int i = 0;
        while (true) {
            if (i > events.size()) {
                break;
            }
            if (events[i]->GetEventType() == eventType && !events[i]->Handled) {
                return std::move(events[i]);
            }
        }
        return nullptr;
    }

    void EventDispatcher::RemoveHandledEvents() {
        events.erase(
            std::remove_if(events.begin(), events.end(), [](const std::unique_ptr<Event>& event) {
                return event->Handled;
                }),
            events.end()
        );
    }

    bool EventDispatcher::isUnhandledEvent(EventType eventType) {
        for (auto& Eventa : events) {
            if (Eventa->GetEventType() == eventType && !Eventa->Handled) {
                return true;
            }
        }
        return false;
    }

    void EventDispatcher::AddEventListener(EventType eventType, EventCallback callback, std::any a) {
        eventCallbacks[eventType].push_back(std::make_pair(callback, a));
    }

    void EventDispatcher::RemoveEventListener(EventType eventType, EventCallback callback) {
        auto& callbacks = eventCallbacks[eventType];
        callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
            [callback](const std::pair<EventCallback, std::any>& listener) {
                return listener.first == callback;
            }), callbacks.end());
    }

    void EventDispatcher::DispatchEvent(GLFWwindow* window) {
        for (auto& aevent : events) {
            if (aevent == NULL) {
                // Probably give a warning that it was empty.
                continue;
            }
            if (!aevent->Handled) {
                auto& callbacks = eventCallbacks[aevent->GetEventType()];
                aevent->Handled = true;
                for (const auto& callback : callbacks) {
                    callback.first(window, std::move(aevent), callback.second);
                }
            }
        }
    }
}