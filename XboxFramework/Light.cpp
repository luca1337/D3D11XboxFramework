#include "Light.h"

//// -- SETTERS -- ////

void Light::SetPosition(const DirectX::XMFLOAT3 & position)
{
	this->position = position;
}

void Light::SetDirection(const DirectX::XMFLOAT3 & direction)
{
	this->direction = direction;
}

void Light::SetAttenuation(const DirectX::XMFLOAT3 & attenuation)
{
	this->attenuation = attenuation;
}

void Light::SetColor(const DirectX::XMFLOAT4 & color)
{
	this->color = color;
}

void Light::SetLightType(const eLightType type)
{
	this->light_type = type;
}

void Light::SetActive(const bool value)
{
	this->is_active = value;
}

void Light::SetIntensity(const float value)
{
	this->intensity = value;
}

void Light::SetRange(const float value)
{
	this->range = value;
}


//// -- GETTERS -- ////

const DirectX::XMFLOAT3 & Light::GetPosition() const
{
	return this->position;
}

const DirectX::XMFLOAT3 & Light::GetDirection() const
{
	return this->direction;
}

const DirectX::XMFLOAT3 & Light::GetAttenuation() const
{
	return this->attenuation;
}

const DirectX::XMFLOAT4 & Light::GetColor() const
{
	return this->color;
}

eLightType Light::GetLightType() const
{
	return this->light_type;
}

bool Light::GetActive() const
{
	return this->is_active;
}

float Light::GetIntensity() const
{
	return this->intensity;
}

float Light::GetRange() const
{
	return this->range;
}
