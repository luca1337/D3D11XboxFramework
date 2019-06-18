#pragma once

#include "Core.h"
#include "Adapter.h"

class Device
{
public:
	Device(std::shared_ptr<Adapter> adapter)
	{
		// this is the ist of features we want to support
		D3D_FEATURE_LEVEL levels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		// this will be filled with the selected feature level
		D3D_FEATURE_LEVEL level;

		// a device is a logical rapresentation of an adapter, allowed to create object
		// (buffers, textures, shaders, ...), in the GPU.
		// a context is a channel for issuing commands
		if (D3D11CreateDevice(adapter->GetDXHandle(), 
							D3D_DRIVER_TYPE_UNKNOWN, 
							nullptr, 
							0,
							levels, 
							_countof(levels), 
							D3D11_SDK_VERSION, 
							(ID3D11Device**)&device,
							&level, 
							(ID3D11DeviceContext**)&context) != S_OK)
		{
			throw std::exception("unable to create device");
		}

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// setup RS (rasterization options)
		D3D11_RASTERIZER_DESC2 r_desc = {};
		r_desc.CullMode = D3D11_CULL_NONE;
		r_desc.FillMode = D3D11_FILL_SOLID;

		CreateRasterizerState(r_desc);
		BindRasterizerState();

		// add stencil here
		D3D11_DEPTH_STENCIL_DESC dsd_desc = {};
		dsd_desc.DepthEnable = true;
		dsd_desc.DepthFunc = D3D11_COMPARISON_LESS;
		dsd_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		if (device->CreateDepthStencilState(&dsd_desc, &dss) != S_OK)
		{
			throw std::exception("unable to create depth stencil state");
		}

		context->OMSetDepthStencilState(dss, 0);

		D3D11_SAMPLER_DESC ss_desc = {};
		ss_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		ss_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		ss_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		ss_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		ss_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		ss_desc.MinLOD = 0;
		ss_desc.MaxLOD = D3D11_FLOAT32_MAX;
		if (device->CreateSamplerState(&ss_desc, &ss) != S_OK)
		{
			throw std::exception("unable to create sampler state");
		}

		context->PSSetSamplers(0, 1, &ss);
	}

	void Draw(UINT vertices_num)
	{
		context->Draw(vertices_num, 0);
	}

	void TurnOffCulling()
	{
		D3D11_RASTERIZER_DESC2 r_desc = {};
		rs->GetDesc2(&r_desc);
		r_desc.CullMode = D3D11_CULL_NONE;

		CreateRasterizerState(r_desc);
		BindRasterizerState();
	}

	void TurnOnBackCulling()
	{
		D3D11_RASTERIZER_DESC2 r_desc = {};
		rs->GetDesc2(&r_desc);
		r_desc.CullMode = D3D11_CULL_BACK;

		CreateRasterizerState(r_desc);
		BindRasterizerState();
	}

	void TurnOnWireframeRendering()
	{
		D3D11_RASTERIZER_DESC2 r_desc = {};
		rs->GetDesc2(&r_desc);
		r_desc.FillMode = D3D11_FILL_WIREFRAME;
		CreateRasterizerState(r_desc);
		BindRasterizerState();
	}

	void TurnOffWireframeRendering()
	{
		D3D11_RASTERIZER_DESC2 r_desc = {};
		rs->GetDesc2(&r_desc);
		r_desc.FillMode = D3D11_FILL_SOLID;
		CreateRasterizerState(r_desc);
		BindRasterizerState();
	}

	void TurnOffZBuffer()
	{
		D3D11_DEPTH_STENCIL_DESC dsd_desc = {};
		dss->GetDesc(&dsd_desc);
		dsd_desc.DepthEnable = false;
		dsd_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		if (device->CreateDepthStencilState(&dsd_desc, &dss) != S_OK)
		{
			throw std::exception("unable to create depth stencil state");
		}

		context->OMSetDepthStencilState(dss, 0);
	}

	void TurnOnZBuffer()
	{
		D3D11_DEPTH_STENCIL_DESC dsd_desc = {};
		dss->GetDesc(&dsd_desc);
		dsd_desc.DepthEnable = true;
		dsd_desc.DepthFunc = D3D11_COMPARISON_LESS;

		if (device->CreateDepthStencilState(&dsd_desc, &dss) != S_OK)
		{
			throw std::exception("unable to create depth stencil state");
		}

		context->OMSetDepthStencilState(dss, 0);
	}

	ID3D11Device5* GetDXHandle()
	{
		return device;
	}

	ID3D11DeviceContext4* GetDXContext()
	{
		return context;
	}

private:
	
	void CreateRasterizerState(D3D11_RASTERIZER_DESC2 rs_desc)
	{
		if (device->CreateRasterizerState2(&rs_desc, &rs) != S_OK)
		{
			throw std::exception("unable to createrasterizer state");
		}
	}

	void BindRasterizerState()
	{
		context->RSSetState(rs);
	}

	ID3D11BlendState* Transparency;
	ID3D11Device5* device;
	ID3D11DeviceContext4* context;
	ID3D11RasterizerState2* rs;
	ID3D11DepthStencilState* dss;
	ID3D11SamplerState* ss;
};
