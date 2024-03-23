/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "Window.h"

namespace Drizzle3D {
    class Layer {
    public:
        Drizzle3D_API Layer() {}

        Drizzle3D_API Layer(Window* window) : name("Layer"), pWindow(window) {}

        Drizzle3D_API virtual ~Layer() = default;
        Drizzle3D_API virtual void OnAttach() { }
        Drizzle3D_API virtual void OnDetach() { }
        Drizzle3D_API virtual void Render() { }

        Drizzle3D_API virtual bool IsShown() const { return show; }
        Drizzle3D_API virtual const std::string& GetName() const { return name; }
        Drizzle3D_API virtual void SetShow(bool value) { show = value; }
    private:
        bool show = false;
        std::string name;
        Window* pWindow = NULL;
    };

    class LayerDispatch {
    public:
        Drizzle3D_API void AddLayer(std::shared_ptr<Layer> layer);
        Drizzle3D_API void RemoveLayerByName(const std::string& name);
        Drizzle3D_API void ShowHideLayerByName(const std::string& name, bool show);
        Drizzle3D_API void PushFront(const std::string& name);
        Drizzle3D_API void PushForward(const std::string& name);
        Drizzle3D_API void PushBack(const std::string& name);
        Drizzle3D_API void PushBackward(const std::string& name);

        Drizzle3D_API void DispatchLayerRender();
        Drizzle3D_API void DispatchLayerDetach();
        Drizzle3D_API void DispatchLayerAttach();

    private:
        std::vector<std::shared_ptr<Layer>> layers;
    };
}