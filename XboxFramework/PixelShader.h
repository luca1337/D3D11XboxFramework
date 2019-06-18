#pragma once

#include "Device.h"

class PixelShader
{
public:
	PixelShader(std::shared_ptr<Device> device, const void* byte_code, SIZE_T byte_code_size) :	device(device)
	{
		if (device->GetDXHandle()->CreatePixelShader(byte_code, byte_code_size, nullptr, &pixel_shader) != S_OK)
		{
			throw std::exception("unable to create pixel shader");
		}
	}

	void Bind()
	{
		device->GetDXContext()->PSSetShader(pixel_shader, nullptr, 0);
	}

private:
	std::shared_ptr<Device> device;
	ID3D11PixelShader* pixel_shader;
	ID3D11InputLayout* input_layout;
};
