#pragma once

#include "DX.h"

class Adapter
{
public:
	Adapter()
	{
		// create a DXGI factory, required for operating system
		// integration, like enumerating adapters and creating swap chains
		auto factory = DX::GetFactory();

		// find the best adapter (hardware and max dedicated unshared memory)
		UINT i = 0;
		adapter = nullptr;
		IDXGIAdapter1* current_adapter = nullptr;
		SIZE_T best_memory = 0;
		while (factory->EnumAdapters1(i++, &current_adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC1 desc;
			if (current_adapter->GetDesc1(&desc) != S_OK)
			{
				OutputDebugString(L"could not get adapter description");
				OutputDebugString(L"\n");
			}
			if (desc.Flags == DXGI_ADAPTER_FLAG_NONE && desc.DedicatedVideoMemory > best_memory)
			{
				best_memory = desc.DedicatedVideoMemory;
				adapter = current_adapter;
			}
			OutputDebugString(desc.Description);
			OutputDebugString(L"\n");
		}

		if (!adapter)
		{
			throw std::exception("unable to find adapter");
		}
	}

	IDXGIAdapter1* GetDXHandle()
	{
		return adapter;
	}

private:
	IDXGIAdapter1* adapter;
};