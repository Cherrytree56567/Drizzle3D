#pragma once
#include "Layer.h"

namespace Drizzle3D {
	class RenderingLayer : public Layer {
    public:
		RenderingLayer(Window* window) : name("3DLayer"), show(true), pWindow(window) {}

        void OnAttach() override;
        void OnDetach() {}
        void Render() override;

        bool IsShown() const { return show; }
        const std::string& GetName() const { return name; }
        void SetShow(bool value) { show = value; }

    protected:
        bool show;

    private:
        unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
        std::string name;
        Window* pWindow;
	};
}