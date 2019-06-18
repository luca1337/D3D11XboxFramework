#pragma once

#include <DirectXMath.h>

enum eLightType : int
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT
};

struct Light
{
	Light() : position(), direction(), attenuation(), color(), light_type(), is_active(false) {}

	// All setters
	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetDirection(const DirectX::XMFLOAT3& direction);
	void SetAttenuation(const DirectX::XMFLOAT3& attenuation);
	void SetColor(const DirectX::XMFLOAT4& color);
	void SetLightType(const eLightType type);
	void SetActive(const bool value);
	void SetIntensity(const float value);
	void SetRange(const float value);

	// All getters
	const DirectX::XMFLOAT3& GetPosition() const;
	const DirectX::XMFLOAT3& GetDirection() const;
	const DirectX::XMFLOAT3& GetAttenuation() const;
	const DirectX::XMFLOAT4& GetColor() const;
	eLightType GetLightType() const;
	bool GetActive() const;
	float GetIntensity() const;
	float GetRange() const;

private:
	DirectX::XMFLOAT3 position;
	float range;
	DirectX::XMFLOAT3 direction;
	float _PAD0;
	DirectX::XMFLOAT3 attenuation;
	float _PAD1;
	DirectX::XMFLOAT4 color;
	eLightType light_type;
	int is_active;
	float intensity;
	float _PAD2;
};

struct LightProperties
{
	DirectX::XMFLOAT4 global_ambient;
	DirectX::XMFLOAT3 eye_position;
	float _pad0;
};