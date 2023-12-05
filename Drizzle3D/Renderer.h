#pragma once
#include "Base.h"
#include <d3dcompiler.h>
#include "D3DException.h"
#include "dxerr.h"

namespace Drizzle3D {
	class Renderer {
	public:
		class Exception : public D3DException {
			using D3DException::D3DException;
		};
		class HrException : public Exception {
		public:
			HrException(int line, const char* file, HRESULT hr) noexcept : Exception(line, file), hr(hr) {}
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;
			std::string GetErrorDescription() const noexcept;
		private:
			HRESULT hr;
		};
		class DRException : public HrException
		{
			using HrException::HrException;
		public:
			const char* GetType() const noexcept override;
		};
		Renderer(HWND hwnd);
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		void RenderFrame();
		void ClearBuffer(float r, float g, float b) noexcept;
	private:	
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	};
}