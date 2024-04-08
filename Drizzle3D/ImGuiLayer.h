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

    class ImGuiLayer : public Layer {
    public:
        Drizzle3D_API ImGuiLayer(RenderingAPI rAPI, Window* window) : renderingAPI(rAPI), name("ImGUI"), show(true), pWindow(window) {}

        typedef void (*ImGUICode)(std::shared_ptr<ImGuiLayer> igui);

        ImGUICode code = [](std::shared_ptr<ImGuiLayer> igui) {};

        Drizzle3D_API void OnAttach() override;
        Drizzle3D_API void OnDetach() { }
        Drizzle3D_API void Render() override;

        Drizzle3D_API bool IsShown() const override { return show; }
        Drizzle3D_API const std::string& GetName() const override { return name; }
        Drizzle3D_API void SetShow(bool value) override { show = value; }
        Drizzle3D_API void setIGUI(std::shared_ptr<ImGuiLayer> ig) { igui = ig; }
        Drizzle3D_API void IterateSliderFloat();
        Drizzle3D_API void GUISliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = NULL, int flags = NULL);
        ImGuiContext* imguiContext = NULL;

    private:
        bool show;
        std::string name;
        Window* pWindow;
        std::shared_ptr<ImGuiLayer> igui;
        std::vector<SliderFloat> SliderFloats;
        RenderingAPI renderingAPI;
        Logging log;
    };
}