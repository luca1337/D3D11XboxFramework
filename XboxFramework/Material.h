#pragma once

struct Material
{
	DirectX::XMFLOAT4  Emissive;       // 16 bytes
	DirectX::XMFLOAT4  Ambient;        // 16 bytes
	DirectX::XMFLOAT4  Diffuse;        // 16 bytes
	DirectX::XMFLOAT4  Specular;       // 16 bytes
	float SpecularPower;			   // 4 bytes
	BOOL UseTexture;				   // 4 bytes
	BOOL UseNormal;					   // 4 bytes
	float _PAD;						   // 4 bytes

	Material() : Emissive(), Ambient(), Diffuse(), Specular(), SpecularPower(), UseTexture(), UseNormal()
	{

	}

	Material(DirectX::XMFLOAT4 emissive, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, float specular_power, BOOL use_texture, BOOL use_normal) :
		Emissive(emissive),
		Ambient(ambient),
		Diffuse(diffuse),
		Specular(specular),
		SpecularPower(specular_power),
		UseTexture(use_texture),
		UseNormal(use_normal)
	{
	}
};

struct MaterialProperties
{
	Material material;
};