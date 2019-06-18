#pragma once

#include "Core.h"

class DX
{
public:
	static IDXGIFactory6* GetFactory()
	{
		static IDXGIFactory6* factory = nullptr;
		if (factory)
			return factory;

		if (CreateDXGIFactory2(0, __uuidof(IDXGIFactory6), (void**)& factory) != S_OK)
		{
			throw std::exception("could not create IDXGI factory");
		}

		return factory;
	}
};