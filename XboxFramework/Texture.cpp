#include "Texture.h"

#include "Engine.h"
#include "Window.h"
#include "ShaderObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(UINT width, UINT height, DXGI_FORMAT format) : width(width), height(height)
{
	// create a new texture
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.ArraySize = 1;
	if (format == DXGI_FORMAT_D24_UNORM_S8_UINT)
	{
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}
	else
	{
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	}
	tex_desc.Format = format;
	tex_desc.Height = height;
	tex_desc.Width = width;
	tex_desc.MipLevels = 1;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;

	if (Engine::Singleton().GetDxDevice()->GetDXHandle()->CreateTexture2D(&tex_desc, nullptr, &texture) != S_OK)
	{
		throw std::exception("unable to create texture");
	}
}

Texture::Texture(std::string file_path)
{
	DXGI_FORMAT format;

	// Load texture from file
	int w, h, ch;

	stbi_uc* pixels = stbi_load(file_path.c_str(), &w, &h, &ch, 4);

	format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (!pixels)
	{
		throw std::exception("unable to retrieve pixels data from texture resource.");
	}

	// Assign w and h
	width = w;
	height = h;

	// create a new texture
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.ArraySize = 1;
	if (format == DXGI_FORMAT_D24_UNORM_S8_UINT)
	{
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}
	else
	{
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	}
	tex_desc.Format = format;
	tex_desc.Height = height;
	tex_desc.Width = width;
	tex_desc.MipLevels = 1;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;

	if (Engine::Singleton().GetDxDevice()->GetDXHandle()->CreateTexture2D(&tex_desc, nullptr, &texture) != S_OK)
	{
		throw std::exception("unable to create texture");
	}

	// UPLOAD IT ON GPU
	Upload(pixels, width * 4, 0);

	// Create our shader object in order to bind our texture
	image = std::make_shared<ShaderObject>(*this);
	//image->Bind(0);
}

Texture::Texture(std::shared_ptr<Window> window)
{
	texture = window->GetDXTexture();
	D3D11_TEXTURE2D_DESC tex_desc;
	texture->GetDesc(&tex_desc);
	width = tex_desc.Width;
	height = tex_desc.Height;
}

void Texture::Upload(void* data, UINT pitch, UINT depth)
{
	Engine::Singleton().GetDxDevice()->GetDXContext()->UpdateSubresource(texture, 0, nullptr, data, pitch, depth);
}

ID3D11Texture2D* Texture::GetDXHandle() { return texture; }

std::shared_ptr<ShaderObject> Texture::GetShaderObject()
{
	return image;
}

UINT Texture::GetWidth() { return width; }

UINT Texture::GetHeight() { return height; }