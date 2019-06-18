#include "Mesh.h"

#include "MeshLoader.h"
#include "GPUBuffer.h"
#include "GPUConstBuffer.h"
#include "Camera.h"
#include "Engine.h"
#include "Graphics.h"
#include "Texture.h"
#include "ShaderObject.h"

Mesh::Mesh(std::string file_path)
{
	MeshLoader::Load(file_path, *this);

	// add 3 buffers always so that we can see the rendered model onto the scene
	AddBuffer(static_cast<UINT>(vertices.size() * sizeof(float) * 3), static_cast<UINT>(sizeof(float) * 3), vertices.data());
	AddBuffer(static_cast<UINT>(normals.size() * sizeof(float) * 3), static_cast<UINT>(sizeof(float) * 3), normals.data());
	AddBuffer(static_cast<UINT>(uvs.size() * sizeof(float) * 2), static_cast<UINT>(sizeof(float) * 2), uvs.data());
	AddBuffer(static_cast<UINT>(tangents.size() * sizeof(float) * 3), static_cast<UINT>(sizeof(float) * 3), tangents.data());
	AddBuffer(static_cast<UINT>(bitangents.size() * sizeof(float) * 3), static_cast<UINT>(sizeof(float) * 3), bitangents.data());

	// MVP buffer for 3D projection local to world
	mvp_buffer = std::make_shared<GPUConstBuffer>(Engine::Singleton().GetDxDevice(), static_cast<UINT>(sizeof(Mvp)));

	// -- NEW -- material buffer -- NEW --
	material_buffer = std::make_shared<GPUConstBuffer>(Engine::Singleton().GetDxDevice(), static_cast<UINT>(sizeof(Material)));

	transform.location = DirectX::XMFLOAT3();
	transform.rotation = DirectX::XMFLOAT3();
	transform.scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
}

void Mesh::SetPosition(const DirectX::XMFLOAT3& pos)
{
	transform.location = pos;
}

void Mesh::SetPosition(float x, float y, float z)
{
	transform.location = DirectX::XMFLOAT3(x, y, z);
}

void Mesh::SetRotation(float x, float y, float z)
{
	transform.rotation = DirectX::XMFLOAT3(x, y, z);
}

void Mesh::Translate(const DirectX::XMFLOAT3& pos)
{
	transform.location.x += pos.x;
	transform.location.y += pos.y;
	transform.location.z += pos.z;
}

void Mesh::Translate(float x, float y, float z)
{
	transform.location.x += x;
	transform.location.y += y;
	transform.location.z += z;
}

void Mesh::Scale(float x, float y, float z)
{
	transform.scale.x += x;
	transform.scale.y += y;
	transform.scale.z += z;
}

void Mesh::Rotate(const DirectX::XMFLOAT3& rot)
{
	transform.rotation.x += rot.x;
	transform.rotation.y += rot.y;
	transform.rotation.z += rot.z;
}

void Mesh::Rotate(float x, float y, float z)
{
	transform.rotation.x += x;
	transform.rotation.y += y;
	transform.rotation.z += z;
}

const DirectX::XMFLOAT3 Mesh::GetPosition() const
{
	return transform.location;
}

void Mesh::Draw(std::shared_ptr<Texture> albedo, std::shared_ptr<Texture> normal_map, std::shared_ptr<MaterialProperties> material)
{
	// bind up camera and ambient light but only once!!!! they must not be changed
	light_properties.eye_position = Graphics::Singleton().GetMainCamera()->GetPosition();
	light_properties.global_ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	Graphics::Singleton().lights_properties_buffer->BindInPixel(2, &light_properties, 1);

	if (material)
	{
		// bind up material
		mat = *material.get();
		material_buffer->BindInPixel(3, &mat, 1);
	}
	else
	{
		// or default material
		mat.material = *default_material.get();
		material_buffer->BindInPixel(3, &mat, 1);
	}

	///// HERE WE NEED TO BIND LIGHT BUFFER /////

	Graphics::Singleton().lights_buffer->BindInPixel(1, Graphics::Singleton().lights.data(), 1);

	UpdateMatrix();

	if (albedo)
	{
		albedo->GetShaderObject()->Bind(0);
	}

	if (normal_map)
	{
		normal_map->GetShaderObject()->Bind(1);
	}

	mvp_buffer->BindInVertex(0, &mvp);

	for (int i = 0; i < buffers.size(); i++)
	{
		buffers[i]->Bind(i);
	}

	Engine::Singleton().GetDxDevice()->GetDXContext()->Draw(vertices_count, 0);

	if (albedo)
	{
		albedo->GetShaderObject()->Unbind(0);
	}

	if (normal_map)
	{
		normal_map->GetShaderObject()->Unbind(1);
	}
}

void Mesh::AddBuffer(UINT size, UINT stride, void* data)
{
	auto buffer = std::make_shared<GPUBuffer>(Engine::Singleton().GetDxDevice(), size, stride, data);
	buffers.push_back(buffer);
}

void Mesh::UpdateMatrix()
{
	// set base values for matrix mult
	translation = DirectX::XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z);
	rotation = DirectX::XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);
	scale = DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);

	// correct the mvp and build it up
	mvp.model = scale * rotation * translation;
	mvp.view = Graphics::Singleton().GetMainCamera()->GetViewMatrix();
	mvp.projection = Graphics::Singleton().GetMainCamera()->GetProjectionMatrix();
}