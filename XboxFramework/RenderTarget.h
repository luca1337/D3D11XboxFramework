#pragma once

#include "Texture.h"
#include "DepthTarget.h"
#include "Engine.h"

class RenderTarget
{
public:
	RenderTarget(std::shared_ptr<Texture> texture) : texture(texture)
	{
		// a render target view is an object able to receive rasterization
		// output (like an opengl framebuffer)
		if (Engine::Singleton().GetDxDevice()->GetDXHandle()->CreateRenderTargetView1(texture->GetDXHandle(), nullptr, &rtv) != S_OK)
		{
			throw std::exception("unable to create render target view");
		}
	}

	void Bind(std::shared_ptr<DepthTarget> depth_target = nullptr)
	{
		depth_view = nullptr;
		if (depth_target)
		{
			depth_view = depth_target->GetDXHandle();
		}
		
		// set the rendering area (viewport)
		D3D11_VIEWPORT viewport = {};
		viewport.Height = (float)texture->GetHeight();
		viewport.Width = (float)texture->GetWidth();
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		Engine::Singleton().GetDxDevice()->GetDXContext()->RSSetViewports(1, &viewport);
	}

	void Clear(std::initializer_list<float> color)
	{
		Engine::Singleton().GetDxDevice()->GetDXContext()->ClearRenderTargetView(rtv, color.begin());
		Engine::Singleton().GetDxDevice()->GetDXContext()->OMSetRenderTargets(1, (ID3D11RenderTargetView**)&rtv, depth_view);
	}

	ID3D11RenderTargetView1* GetDXHandle() { return rtv; }

private:
	ID3D11DepthStencilView* depth_view;
	ID3D11RenderTargetView1* rtv;
	std::shared_ptr<Texture> texture;
};
