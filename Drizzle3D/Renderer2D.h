#pragma once
#include "Layer.h"
#include "base.h"
#include "ResourceManager.h"
#include "Flags.h"

namespace Drizzle3D {
	class RenderingLayer2D : public Layer {
	public:
        Drizzle3D_API RenderingLayer2D(RenderingAPI rAPI, std::shared_ptr<Window> window, std::shared_ptr<ResourceManager> resmgr);

        Drizzle3D_API void OnAttach() override;
        Drizzle3D_API void OnDetach() override {}
        Drizzle3D_API void Render() override;

        Drizzle3D_API bool IsShown() const override { return show; }
        Drizzle3D_API const std::string& GetName() const override { return name; }
        Drizzle3D_API void SetShow(bool value) override { show = value; }

        Drizzle3D_API Flags* GetFlags() { return &flags; }
    private:
        bool show;
        Flags flags;
        std::string name;
        std::shared_ptr<Window> pWindow;
        std::shared_ptr<ResourceManager> resourcemgr;
        Logging log;
        RenderingAPI renderingAPI;
	};
}