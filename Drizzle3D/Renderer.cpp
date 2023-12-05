#include "Renderer.h"

#define RDX_Exception(hrcall) if( FAILED( hr = (hrcall) ) ) throw Renderer::HrException( __LINE__,__FILE__,hr )
#define RDX_DRException(hr) Renderer::DRException( __LINE__,__FILE__,(hr) )
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS


namespace Drizzle3D {
	Renderer::Renderer(HWND hwnd) {
		HRESULT hr;

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
		sd.OutputWindow = hwnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		RDX_Exception(D3D11CreateDeviceAndSwapChain(
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
		));

		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
		RDX_Exception(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		RDX_Exception(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
	}
	void Renderer::RenderFrame() {
		HRESULT hr;
		if (FAILED(hr = pSwap->Present(1u, 0u))) {
			if (hr == DXGI_ERROR_DEVICE_REMOVED) {
				throw RDX_DRException(pDevice->GetDeviceRemovedReason());
			} else {
				RDX_Exception(hr);
			}
		}
	}
	void Renderer::ClearBuffer(float r, float g, float b) noexcept {
		const float color[] = {r, g, b, 1.0f};
		pContext->ClearRenderTargetView(pTarget.Get(), color);
	}

	const char* Renderer::HrException::what() const noexcept {
		std::ostringstream oss;
		oss << GetType() << std::endl << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl << "[Error String] " << GetErrorString() << std::endl << "[Description] " << GetErrorDescription() << std::endl << GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* Renderer::HrException::GetType() const noexcept {
		return "Drizzle3D Graphics Exception";
	}

	HRESULT Renderer::HrException::GetErrorCode() const noexcept {
		return hr;
	}

	std::string Renderer::HrException::GetErrorString() const noexcept {
		//May not convert properly.
		const WCHAR* a = DXGetErrorString(hr);
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(a);
	}

	std::string Renderer::HrException::GetErrorDescription() const noexcept {
		char buf[512];
		int len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
		WCHAR* wideString = new WCHAR[len];
		MultiByteToWideChar(CP_UTF8, 0, buf, -1, wideString, len);
		DXGetErrorDescription(hr, wideString, sizeof(buf));
		return buf;
	}


	const char* Renderer::DRException::GetType() const noexcept {
		return "Drizzle3D Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
	}
}