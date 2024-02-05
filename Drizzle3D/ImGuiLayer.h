/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Window.h"
#include "Layer.h"

namespace Drizzle3D {
    struct SliderFloat {
        const char* label;
        float* v;
        float v_min;
        float v_max;
        const char* format = NULL;
        ImGuiSliderFlags flags = NULL;
    };

    class Drizzle3D_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer(Window* window) : name("ImGUI"), show(true), pWindow(window) {}

        typedef void (*ImGUICode)(std::shared_ptr<ImGuiLayer> igui);

        ImGUICode code = [](std::shared_ptr<ImGuiLayer> igui) {};

        void OnAttach() override;
        void OnDetach() { }
        void Render() override;

        bool IsShown() const override { return show; }
        const std::string& GetName() const override { return name; }
        void SetShow(bool value) override { show = value; }
        void setIGUI(std::shared_ptr<ImGuiLayer> ig) { igui = ig; }
        void IterateSliderFloat();
        void GUISliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = NULL, int flags = NULL);

    private:
        bool show;
        std::string name;
        Window* pWindow;
        std::shared_ptr<ImGuiLayer> igui;
        std::vector<SliderFloat> SliderFloats;
    };
}