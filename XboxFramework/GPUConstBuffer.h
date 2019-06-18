#pragma once

#pragma once

#include "Device.h"
#include <memory>

class GPUConstBuffer
{
public:
	GPUConstBuffer(std::shared_ptr<Device> device, UINT size, const void* data = nullptr) : device(device), buffer(nullptr)
	{
		D3D11_BUFFER_DESC buf_desc = {};
		buf_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buf_desc.ByteWidth = size;
		buf_desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA* buf_data_ptr = nullptr;

		if (data)
		{
			// like textures, but without pitching
			D3D11_SUBRESOURCE_DATA buf_data = {};
			buf_data.pSysMem = data;
			buf_data_ptr = &buf_data;
		}

		if (device->GetDXHandle()->CreateBuffer(&buf_desc, buf_data_ptr, &buffer) != S_OK)
		{
			throw std::exception("unable to create constant buffer");
		}
	}

	void BindInVertex(UINT gpu_register, void* data)
	{
		device->GetDXContext()->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
		device->GetDXContext()->VSSetConstantBuffers(gpu_register, 1, &buffer);
	}

	void BindInPixel(UINT gpu_register, void* data, UINT num_buffers)
	{
		device->GetDXContext()->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
		device->GetDXContext()->PSSetConstantBuffers(gpu_register, num_buffers, &buffer);
	}

	ID3D11Buffer* GetDXHandle() { return buffer; }

private:
	ID3D11Buffer* buffer;
	std::shared_ptr<Device> device;
};
