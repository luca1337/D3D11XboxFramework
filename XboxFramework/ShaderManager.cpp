#include "ShaderManager.h"

#include "PixelShader.h"
#include "VertexShader.h"
#include "ShaderLayoutDesc.h"
#include "Engine.h"

std::map<std::string, std::shared_ptr<VertexShader>> ShaderManager::vertex_shaders;
std::map<std::string, std::shared_ptr<PixelShader>> ShaderManager::pixel_shaders;

void ShaderManager::CreateAndAddVertexShaderResource(D3D11_INPUT_ELEMENT_DESC* shader_desc, std::string vertex_name, const void* vertex_byte_code, UINT vertex_byte_code_sz, UINT shader_desc_sz)
{
	auto vertex_it = vertex_shaders.find(vertex_name);

	if (vertex_it != vertex_shaders.end())
	{
		throw std::exception("element already exists in memory, please use GetResource() method to retrieve it instead.");
	}

	std::shared_ptr<VertexShader> vertex_shader	= std::make_shared<VertexShader>(Engine::Singleton().GetDxDevice(),
																								vertex_byte_code,
																								vertex_byte_code_sz,
																								shader_desc,
																								shader_desc_sz);
	vertex_shaders.emplace(vertex_name, vertex_shader);
}

void ShaderManager::CreateAndAddPixelShaderResource(std::string pixel_name, const void* pixel_byte_code, UINT pixel_byte_code_sz)
{
	auto pixel_it = pixel_shaders.find(pixel_name);

	if (pixel_it != pixel_shaders.end())
	{
		throw std::exception("element already exists in memory, please use GetResource() method to retrieve it instead.");
	}

	std::shared_ptr<PixelShader> pixel_shader
		= std::make_shared<PixelShader>(
		Engine::Singleton().GetDxDevice(),
		pixel_byte_code,
		pixel_byte_code_sz);

	pixel_shaders.emplace(pixel_name, pixel_shader);
}

std::shared_ptr<VertexShader> ShaderManager::GetVertexShaderResourceFromMemory(std::string vertex_name)
{
	std::shared_ptr<VertexShader> result;

	for (auto const& [key, val] : vertex_shaders)
	{
		if (key == vertex_name)
		{
			result = val;
			break;
		}
	}

	return result;
}

std::shared_ptr<PixelShader> ShaderManager::GetPixelShaderResourceFromMemory(std::string pixel_name)
{
	std::shared_ptr<PixelShader> result;

	for (auto const& [key, val] : pixel_shaders)
	{
		if (key == pixel_name)
		{
			result = val;
			break;
		}
	}

	return result;
}

std::vector<std::string> ShaderManager::GetVertexShadersListNames()
{
	std::vector<std::string> names;

	for (auto const& [key, value] : vertex_shaders)
	{
		names.push_back(key);
	}

	return names;
}

std::vector<std::string> ShaderManager::GetPixelShadersListNames()
{
	std::vector<std::string> names;

	for (auto const& [key, value] : pixel_shaders)
	{
		names.push_back(key);
	}

	return names;
}
