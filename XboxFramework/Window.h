#pragma once

#include "Device.h"

class Window
{
public:
	Window(std::shared_ptr<Device> device, IUnknown* windowPtr, UINT width, UINT height) : device(device), windowPtr(windowPtr), width(width), height(height), swap_chain(nullptr)
	{
		DXGI_SWAP_CHAIN_DESC1 sc_desc = {};
		sc_desc.BufferCount = 2;
		sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sc_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // or DXGI_FORMAT_R16G16B16A16_FLOAT
		sc_desc.SampleDesc.Count = 1;
		sc_desc.SampleDesc.Quality = 0;
		sc_desc.Width = width;
		sc_desc.Height = height;
		sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		if (DX::GetFactory()->CreateSwapChainForCoreWindow(
									device->GetDXHandle(),
									windowPtr,
									&sc_desc,
									nullptr,
									(IDXGISwapChain1**)&swap_chain) != S_OK)
		{
			throw std::exception("unable to create the swap chain");
		}
	}

	void Present()
	{

		// bit blit the back buffer to the front buffer, waiting for vsync
		swap_chain->Present(1, 0);
	}

	ID3D11Texture2D* GetDXTexture()
	{
		ID3D11Texture2D* swap_chain_texture = nullptr;
		if (swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&swap_chain_texture) != S_OK)
		{
			throw std::exception("unable to get the swap chain texture");
		}
		return swap_chain_texture;
	}

	std::shared_ptr<Device> GetDevice()
	{
		return device;
	}

private:
	std::shared_ptr<Device> device;
	IUnknown* windowPtr;
	IDXGISwapChain4* swap_chain;
	UINT width;
	UINT height;
};