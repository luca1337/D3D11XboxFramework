#pragma once

#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	void SetCameraProjection(float fov, float aspect_ratio, float z_near = 0.01f, float z_far = 1000.0f);

	const DirectX::XMMATRIX& GetViewMatrix() const;
	const DirectX::XMMATRIX& GetProjectionMatrix() const;

	void SetPosition(const DirectX::XMVECTOR& pos);
	void SetPosition(float x, float y, float z);

	void SetRotation(const DirectX::XMVECTOR& rot);
	void SetRotation(float roll, float pitch, float yaw);

	void Translate(float x, float y, float z);
	void Translate(const DirectX::XMVECTOR& pos);

	void Rotate(float roll, float pitch, float yaw);
	void Rotate(const DirectX::XMVECTOR& rot);

	const DirectX::XMFLOAT3& GetPosition() const;
	const DirectX::XMFLOAT3& GetRotation() const;

	const DirectX::XMVECTOR& GetForwardVector();
	const DirectX::XMVECTOR& GetUpVector();
	const DirectX::XMVECTOR& GetDownVector();
	const DirectX::XMVECTOR& GetLeftVector();
	const DirectX::XMVECTOR& GetRightVector();
	const DirectX::XMVECTOR& GetBackwardVector();

private:

	void UpdateCameraMatrix();

	// Camera Vectors
	DirectX::XMVECTOR position_vector;
	DirectX::XMFLOAT3 position;
	DirectX::XMVECTOR rotation_vector;
	DirectX::XMFLOAT3 rotation;

	// 3D Projection
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;

	const DirectX::XMVECTOR UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DOWN_VECTOR = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR BACKWRD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

	DirectX::XMVECTOR up;
	DirectX::XMVECTOR down;
	DirectX::XMVECTOR forward;
	DirectX::XMVECTOR left;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR backward;
};