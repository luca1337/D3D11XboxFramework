#pragma once

#include "Core.h"
#include "Device.h"

class GPUBuffer
{
public:
	GPUBuffer(std::shared_ptr<Device> device, UINT size, UINT stride, const void* data = nullptr) : device(device), stride(stride)
	{
		D3D11_BUFFER_DESC b_desc = {};
		b_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		b_desc.ByteWidth = size;
		b_desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA* buf_data_ptr = nullptr;

		if (data)
		{
			// like texture but without pitching etc..
			D3D11_SUBRESOURCE_DATA buf_data = {};
			buf_data.pSysMem = data;
			buf_data_ptr = &buf_data;
		}

		if (device->GetDXHandle()->CreateBuffer(&b_desc, buf_data_ptr, &buffer) != S_OK)
		{
			throw std::exception("unable to create buffer");
		}
	}

	void Bind(UINT slot)
	{
		UINT strides = { stride }; // how many bytes to read foreach vertex
		UINT offsets = { 0 };
		
		// NOTE: IASetVertexBuffers takes an array of buffers, strides and offsets
		// but we can call it multiple times for different slots

		device->GetDXContext()->IASetVertexBuffers(slot, 1, &buffer, &strides, &offsets);
	}

	~GPUBuffer()
	{
		buffer->Release();
	}

	ID3D11Buffer* GetDXHandle()
	{
		return buffer;
	}

private:
	ID3D11Buffer* buffer;
	std::shared_ptr<Device> device;
	UINT stride;
};