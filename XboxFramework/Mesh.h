#pragma once

#include "Mvp.h"
#include "Transform.h"
#include <string>
#include <vector>
#include "Core.h"
#include "Material.h"
#include "Light.h"

class GPUBuffer;
class GPUConstBuffer;
class Texture;

class Mesh
{
public:
	Mesh(std::string file_path);

	void SetPosition(const DirectX::XMFLOAT3& pos);

	void SetPosition(float x, float y, float z);

	void SetRotation(float x, float y, float z);

	void Translate(const DirectX::XMFLOAT3& pos);

	void Translate(float x, float y, float z);

	void Scale(float x, float y, float z);

	void Rotate(const DirectX::XMFLOAT3& rot);

	void Rotate(float x, float y, float z);

	const DirectX::XMFLOAT3 GetPosition() const;

	void Draw(std::shared_ptr<Texture> albedo, std::shared_ptr<Texture> normal_map, std::shared_ptr<MaterialProperties> material);

private:
	void AddBuffer(UINT size, UINT stride, void* data);

	void UpdateMatrix();

	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> uvs;
	std::vector<DirectX::XMFLOAT3> tangents;
	std::vector<DirectX::XMFLOAT3> bitangents;

	DirectX::XMMATRIX translation;
	DirectX::XMMATRIX rotation;
	DirectX::XMMATRIX scale;

	Transform transform;
	Mvp mvp;

	std::vector<std::shared_ptr<GPUBuffer>> buffers;
	std::shared_ptr<GPUConstBuffer> mvp_buffer;
	std::shared_ptr<GPUConstBuffer> material_buffer;

	UINT vertices_count;

	const std::shared_ptr<Material> default_material = std::make_shared<Material>
		(
			DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
			128.0f,
			false,
			false
		);

	LightProperties light_properties;
	MaterialProperties mat;

	friend class MeshLoader;
};