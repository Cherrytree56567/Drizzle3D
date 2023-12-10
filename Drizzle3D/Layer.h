#pragma once
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "Window.h"

namespace Drizzle3D {
    class Layer {
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

    class LayerDispatch {
    public:
        void AddLayer(Layer* layer);
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
        std::vector<Layer*> layers;
        
    };
}