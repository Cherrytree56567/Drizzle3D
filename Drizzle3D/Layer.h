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

        Layer(Window* window) : name("Layer"), show(true), pWindow(window) {}

        virtual ~Layer() = default;
        virtual void OnAttach() { }
        virtual void OnDetach() { }
        virtual void Render() { }

        bool IsShown() const { return show; }
        const std::string& GetName() const { return name; }
        void SetShow(bool value) { show = value; }

    protected:
        bool show;

    private:
        std::string name;
        Window* pWindow;
    };

    class LayerDispatch {
    public:
        void AddLayer(Layer* layer);
        void RemoveLayerByName(const std::string& name);
        void ShowHideLayerByName(const std::string& name, bool show);
        void PushFront(const std::string& layerName);
        void PushBack(const std::string& layerName);
        void PushForward(const std::string& layerName);
        void PushBackward(const std::string& layerName);
        void ChangeLayerOrderByName(const std::string& name, size_t newIndex);

        void DispatchLayerRender();
        void DispatchLayerDetach();
        void DispatchLayerAttach();

    private:
        std::vector<Layer*> layers;
        
    };
}