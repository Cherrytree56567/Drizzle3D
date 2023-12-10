#pragma once
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Window.h"
#include "Layer.h"

namespace Drizzle3D {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer(Window* window) : name("ImGUI"), show(true), pWindow(window) {}

        typedef void (*ImGUICode)(ImGuiLayer* imguir);

        ImGUICode code = [](ImGuiLayer* imguir) {};

        void OnAttach() override;
        void OnDetach() { }
        void Render() override;

        bool IsShown() const override { return show; }
        const std::string& GetName() const override { return name; }
        void SetShow(bool value) override { show = value; }

    private:
        bool show;
        std::string name;
        Window* pWindow;
    };
}