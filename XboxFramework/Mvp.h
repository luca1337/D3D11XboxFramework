#pragma once

#include <DirectXMath.h>

struct Mvp
{
	DirectX::XMMATRIX model;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};