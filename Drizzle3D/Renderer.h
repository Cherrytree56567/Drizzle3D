#pragma once
#include <Windows.h>
#include <d3d11.h>

namespace Drizzle3D {
	class Renderer {
	public:
		Renderer(HWND hwnd);
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		~Renderer();
		void RenderFrame();
		void ClearBuffer(float r, float g, float b) noexcept;
	private:
		HWND hWnd;
		ID3D11Device* pDevice = nullptr;
		IDXGISwapChain* pSwap = nullptr;
		ID3D11DeviceContext* pContext = nullptr;
		ID3D11RenderTargetView* pTarget = nullptr;
	};
}