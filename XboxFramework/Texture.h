#pragma once

#include <string>
#include "Core.h"

class Window;
class ShaderObject;

class Texture : public std::enable_shared_from_this<Texture>
{
public:
	Texture(UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	/*
	@ Used to create textures for mesh, if you want to have a mesh with a texture use this one
	@ Other methods are not working to render images on meshes.
	*/
	Texture(std::string file_path);

	Texture(std::shared_ptr<Window> window);

	void Upload(void* data, UINT pitch, UINT depth);

	ID3D11Texture2D* GetDXHandle();

	std::shared_ptr<ShaderObject> GetShaderObject();

	UINT GetWidth();
	UINT GetHeight();

private:
	std::shared_ptr<ShaderObject> image;
	ID3D11Texture2D* texture;
	UINT width;
	UINT height;
};
