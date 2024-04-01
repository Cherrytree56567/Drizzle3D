#include "RenderingLayer2D.h"

namespace Drizzle3D {
    RenderingLayer2D::RenderingLayer2D(Window* window, std::shared_ptr<ResourceManager> resmgr) : name("3DLayer"), show(true), pWindow(window), resourcemgr(resmgr) {
        flags.AddFlag("Show", show);
    }

    void RenderingLayer2D::OnAttach() {

    }

    void RenderingLayer2D::Render() {

    }
}