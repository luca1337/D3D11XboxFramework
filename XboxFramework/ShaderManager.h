#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>
#include "Core.h"

class VertexShader;
class PixelShader;
struct ShaderLayoutDesc;

class ShaderManager
{

public:
	static void CreateAndAddVertexShaderResource(D3D11_INPUT_ELEMENT_DESC* shader_desc, std::string vertex_name, const void* vertex_byte_code, UINT vertex_byte_code_sz, UINT shader_desc_sz);
	static void CreateAndAddPixelShaderResource(std::string pixel_name, const void* pixel_byte_code, UINT pixel_byte_code_sz);

	static std::shared_ptr<VertexShader> GetVertexShaderResourceFromMemory(std::string vertex_name);
	static std::shared_ptr<PixelShader> GetPixelShaderResourceFromMemory(std::string pixel_name);

	static std::vector<std::string> GetVertexShadersListNames();
	static std::vector<std::string> GetPixelShadersListNames();

private:
	static std::map<std::string, std::shared_ptr<VertexShader>> vertex_shaders;
	static std::map<std::string, std::shared_ptr<PixelShader>> pixel_shaders;
};