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
    class Drizzle3D_API Layer {
    public:
        Layer() {}

        Layer(Window* window) : name("Layer"), pWindow(window) {}

        virtual ~Layer() = default;
        virtual void OnAttach() { }
        virtual void OnDetach() { }
        virtual void Render() { }

        virtual bool IsShown() const { return show; }
        virtual const std::string& GetName() const { return name; }
        virtual void SetShow(bool value) { show = value; }
    private:
        bool show;
        std::string name;
        Window* pWindow;
    };

    class Drizzle3D_API LayerDispatch {
    public:
        void AddLayer(std::shared_ptr<Layer> layer);
        void RemoveLayerByName(const std::string& name);
        void ShowHideLayerByName(const std::string& name, bool show);
        void PushFront(const std::string& name);
        void PushForward(const std::string& name);
        void PushBack(const std::string& name);
        void PushBackward(const std::string& name);

        void DispatchLayerRender();
        void DispatchLayerDetach();
        void DispatchLayerAttach();

    private:
        std::vector<std::shared_ptr<Layer>> layers;
    };
}