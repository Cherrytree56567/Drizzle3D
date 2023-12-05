#include "Renderer.h"

namespace Drizzle3D {
	Renderer::Renderer(HWND hwnd) : hWnd(hwnd) {

		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = hWnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwap,
			&pDevice,
			nullptr,
			&pContext
		);

		ID3D11Resource* pBackBuffer = nullptr;
		pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget);
		pBackBuffer->Release();
	}
	Renderer::~Renderer() {
		if (pContext != nullptr)
		{
			pContext->Release();
		}
		if (pSwap != nullptr)
		{
			pSwap->Release();
		}
		if (pDevice != nullptr)
		{
			pDevice->Release();
		}
	}
	void Renderer::RenderFrame() {
		pSwap->Present(1u, 0u);
	}
	void Renderer::ClearBuffer(float r, float g, float b) noexcept {
		const float color[] = {r, g, b, 1.0f};
		pContext->ClearRenderTargetView(pTarget, color);
	}
}