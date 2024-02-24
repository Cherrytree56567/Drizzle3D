#pragma once
#include "base.h"
#include "Layer.h"
#include <d3d.h>
#include <d3dcompiler.h>

namespace Drizzle3D {
	class Drizzle3D_API RenderingLayer : public Layer {
	public:
		RenderingLayer(Window* window) : name("RenderingLayer"), show(true), pWindow(window) {}

		void OnAttach() override;
		void OnDetach() { }
		void Render() override;

	private:
		bool show;
		std::string name;
		Window* pWindow;
		std::shared_ptr<RenderingLayer> rlay;

        // Vertex shader source code
		// Vertex shader source code
		const char* vertexShaderSource =
			"struct VS_INPUT\n"
			"{\n"
			"    float3 position : POSITION;\n"
			"};\n"
			"struct VS_OUTPUT\n"
			"{\n"
			"    float4 position : SV_POSITION;\n"
			"};\n"
			"VS_OUTPUT main(VS_INPUT input)\n"
			"{\n"
			"    VS_OUTPUT output;\n"
			"    output.position = float4(input.position, 1.0f);\n" // Perform basic transformation
			"    return output;\n"
			"}\n";


		// Pixel shader source code
		const char* pixelShaderSource =
			"float4 main() : SV_TARGET\n"
			"{\n"
			"    return float4(1.0f, 0.0f, 0.0f, 1.0f); // Red color\n"
			"}\n";

		ID3D11Device* device = pWindow->returnDX11Device();
		ID3D11DeviceContext* deviceContext = pWindow->returnDX11DeviceContext();
		ID3DBlob* vertexShaderBlob;
		ID3DBlob* pixelShaderBlob;
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		IDXGISwapChain* swapChain = pWindow->returnDXGISwapChain();
		ID3D11RenderTargetView* swapChainRenderTargetView = pWindow->returnD3D11RenderTargetView();
		ID3D11InputLayout* inputLayout;
	};
}