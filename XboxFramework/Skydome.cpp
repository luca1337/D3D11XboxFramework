#include "SkyDome.h"

#include "Graphics.h"
#include "Device.h"
#include "Engine.h"
#include "Mesh.h"
#include "Camera.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "ShaderManager.h"

Skydome::Skydome(const std::string& skydome_file)
{
	skydome_mesh = std::make_shared<Mesh>(skydome_file);
	skydome_mesh->Scale(2.5f, 1.5f, 2.5f);

	skydome_vs = ShaderManager::GetVertexShaderResourceFromMemory("skydome_vertex");
	skydome_ps = ShaderManager::GetPixelShaderResourceFromMemory("skydome_pixel");

	basic_vs = ShaderManager::GetVertexShaderResourceFromMemory("basic_vertex");
	basic_ps = ShaderManager::GetPixelShaderResourceFromMemory("basic_pixel");
}

void Skydome::Render()
{
	// !!!! NOTE !!!!
	// Skydome must be rendered before every other object, due to some trick we 
	// use to create the far away illusion. we disable the Z-Buffer while rendering
	// and we enable it at the end of the skydome render process, if the skydome
	// will be rendered after some object then that object will not be rendered.

	// Bind shaders for the skydome

	Engine::Singleton().GetDxDevice()->TurnOffZBuffer();

	skydome_vs->Bind();
	skydome_ps->Bind();
	skydome_mesh->Draw(nullptr, nullptr, nullptr);

	Engine::Singleton().GetDxDevice()->TurnOnZBuffer();

	skydome_mesh->SetPosition(
		Graphics::Singleton().GetMainCamera()->GetPosition().x,
		Graphics::Singleton().GetMainCamera()->GetPosition().y,
		Graphics::Singleton().GetMainCamera()->GetPosition().z
	);

	basic_vs->Bind();
	basic_ps->Bind();
}
