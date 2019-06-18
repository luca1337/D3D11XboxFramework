#include "Camera.h"

Camera::Camera() : view(DirectX::XMMATRIX()), projection(DirectX::XMMATRIX())
{
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	position_vector = DirectX::XMLoadFloat3(&position);

	rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation_vector = DirectX::XMLoadFloat3(&rotation);
}

void Camera::SetCameraProjection(float fov, float aspect_ratio, float z_near, float z_far)
{
	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect_ratio, z_near, z_far);
}

const DirectX::XMMATRIX& Camera::GetViewMatrix() const
{
	return view;
}

const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const
{
	return projection;
}

void Camera::SetPosition(const DirectX::XMVECTOR& pos)
{
	DirectX::XMStoreFloat3(&position, pos);
	position_vector = pos;
	UpdateCameraMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
	position_vector = DirectX::XMLoadFloat3(&position);
	UpdateCameraMatrix();
}

void Camera::SetRotation(const DirectX::XMVECTOR& rot)
{
	DirectX::XMStoreFloat3(&rotation, rot);
	rotation_vector = rot;
	UpdateCameraMatrix();
}

void Camera::SetRotation(float roll, float pitch, float yaw)
{
	rotation = DirectX::XMFLOAT3(roll, pitch, yaw);
	rotation_vector = DirectX::XMLoadFloat3(&rotation);
	UpdateCameraMatrix();
}

void Camera::Translate(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
	position_vector = DirectX::XMLoadFloat3(&position);
	UpdateCameraMatrix();
}

void Camera::Translate(const DirectX::XMVECTOR& pos)
{
	DirectX::XMFLOAT3 tmp;
	DirectX::XMStoreFloat3(&tmp, pos);
	position.x += tmp.x;
	position.y += tmp.y;
	position.z += tmp.z;
	position_vector = DirectX::XMLoadFloat3(&position);
	UpdateCameraMatrix();
}

void Camera::Rotate(float roll, float pitch, float yaw)
{
	rotation.x += roll;
	rotation.y += pitch;
	rotation.z += yaw;
	rotation_vector = DirectX::XMLoadFloat3(&rotation);
	UpdateCameraMatrix();
}

void Camera::Rotate(const DirectX::XMVECTOR& rot)
{
}

const DirectX::XMFLOAT3& Camera::GetPosition() const
{
	return position;
}

const DirectX::XMFLOAT3& Camera::GetRotation() const
{
	return rotation;
}

const DirectX::XMVECTOR& Camera::GetForwardVector()
{
	return forward;
}

const DirectX::XMVECTOR& Camera::GetUpVector()
{
	return up;
}

const DirectX::XMVECTOR & Camera::GetDownVector()
{
	return down;
}

const DirectX::XMVECTOR& Camera::GetLeftVector()
{
	return left;
}

const DirectX::XMVECTOR& Camera::GetRightVector()
{
	return right;
}

const DirectX::XMVECTOR& Camera::GetBackwardVector()
{
	return backward;
}

void Camera::UpdateCameraMatrix()
{
	// get current camera rotation matrix
	DirectX::XMMATRIX camera_rotation = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	DirectX::XMVECTOR camera_focus = DirectX::XMVector3TransformCoord(FORWARD_VECTOR, camera_rotation);

	DirectX::XMVECTOR new_cam_focus = DirectX::XMVectorAdd(camera_focus, position_vector);

	DirectX::XMVECTOR direction_up = DirectX::XMVector3TransformCoord(UP_VECTOR, camera_rotation);

	view = DirectX::XMMatrixLookAtLH(position_vector, new_cam_focus, direction_up);

	DirectX::XMMATRIX view_rotation = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	this->up = DirectX::XMVector3TransformCoord(UP_VECTOR, view_rotation);
	this->down = DirectX::XMVector3TransformCoord(DOWN_VECTOR, view_rotation);
	this->forward = DirectX::XMVector3TransformCoord(FORWARD_VECTOR, view_rotation);
	this->left = DirectX::XMVector3TransformCoord(LEFT_VECTOR, view_rotation);
	this->right = DirectX::XMVector3TransformCoord(RIGHT_VECTOR, view_rotation);
	this->backward = DirectX::XMVector3TransformCoord(BACKWRD_VECTOR, view_rotation);
}