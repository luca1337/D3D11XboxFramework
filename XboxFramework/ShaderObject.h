#pragma once

#include "Texture.h"
#include "Engine.h"
#include "Device.h"

class ShaderObject
{
public:
	ShaderObject(Texture texture) : texture(texture)
	{
		if (Engine::Singleton().GetDxDevice()->GetDXHandle()->CreateShaderResourceView1(texture.GetDXHandle(), nullptr, &srv) != S_OK)
		{
			throw std::exception("unable to create shader resource view");
		}
	}

	void Bind(UINT gpu_register)
	{
		Engine::Singleton().GetDxDevice()->GetDXContext()->PSSetShaderResources(gpu_register, 1, (ID3D11ShaderResourceView**)&srv);
	}

	void Unbind(UINT gpu_register)
	{
		ID3D11ShaderResourceView* nullsrv[1] = { nullptr };
		Engine::Singleton().GetDxDevice()->GetDXContext()->PSSetShaderResources(gpu_register, 1, nullsrv);
	}

private:
	ID3D11ShaderResourceView1* srv;
	Texture texture;
};
