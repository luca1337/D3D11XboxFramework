#pragma once

#include "winrt/Windows.ApplicationModel.Core.h"
#include "winrt/Windows.UI.Core.h"
#include "winrt/Windows.UI.ViewManagement.h"

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::System;

#include "ShaderObject.h"
#include "Mesh.h"
#include "Light.h"
#include "Transform.h"
#include "GPUConstBuffer.h"
#include "Camera.h"
#include "ShaderObject.h"
#include "Texture.h"
#include <time.h>
#include "SkyDome.h"
#include "ShaderManager.h"
#include "Material.h"

#include <GeometricPrimitive.h>
#include <Effects.h>

#include <SpriteBatch.h>

#include <Keyboard.h>

#include "Engine.h"
#include "Graphics.h"

double PCFreq = 0.0;
__int64 CounterStart = 0;

float RandomFloat(float min, float max)
{
	// this  function assumes max > min, you may want 
	// more robust error checking for a non-debug build
	assert(max > min);
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = max - min;
	return (random * range) + min;
}

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!" << std::endl;

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

class ViewProvider : public winrt::implements<ViewProvider, IFrameworkView>
{
public:
	// IFrameworkView methods
	void Initialize(CoreApplicationView const& applicationView)
	{
		srand((unsigned(time(NULL))));
	}

	void Uninitialize()
	{

	}

	void SetWindow(CoreWindow const& window)
	{
		window.Activate();

		rect = window.Bounds();

		auto windowPtr = static_cast<::IUnknown*>(winrt::get_abi(window));

		// fix b button
		auto navigation = SystemNavigationManager::GetForCurrentView();
		navigation.BackRequested([](auto, auto args)
		{
			args.Handled(true);
		});

		// Init engine
		Engine& engine = Engine::Singleton();

		// init graphics pipeline
		Graphics& graphics = Graphics::Singleton();
		graphics.Setup(windowPtr, rect.Width / rect.Height);

		Light light0;
		Light light1;
		Light light2;
		Light light3;
		Light light4;

		// -- NEW -- SETUP SOME LIGHT -- NEW --
		light0.SetLightType(eLightType::POINT_LIGHT);
		light0.SetPosition({ 0.0f, 0.0f, 60.0f });
		light0.SetRange(2.0f);
		light0.SetAttenuation({ 1.0f, 0.08f, 0.0f });
		light0.SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		light0.SetIntensity(2.0f);
		light0.SetActive(true);

		light1.SetLightType(eLightType::POINT_LIGHT);
		light1.SetPosition({ 0.0f, 0.0f, -30.0f });
		light1.SetRange(4.0f);
		light1.SetAttenuation({ 1.0f, 0.08f, 0.0f });
		light1.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		light1.SetIntensity(2.0f);
		light1.SetActive(true);

		light2.SetLightType(eLightType::POINT_LIGHT);
		light2.SetPosition({ 60.0f, 0.0f, 0.0f });
		light2.SetRange(2.0f);
		light2.SetAttenuation({ 1.0f, 0.08f, 0.0f });
		light2.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
		light2.SetIntensity(2.0f);
		light2.SetActive(true);

		light3.SetLightType(eLightType::POINT_LIGHT);
		light3.SetPosition({ -60.0f, 0.0f, 0.0f });
		light3.SetRange(3.0f);
		light3.SetAttenuation({ 1.0f, 0.08f, 0.0f });
		light3.SetColor({ 1.0f, 0.0f, 1.0f, 1.0f });
		light3.SetIntensity(2.0f);
		light3.SetActive(true);

		light4.SetLightType(eLightType::DIRECTIONAL_LIGHT);
		light4.SetDirection({ 0.0f, -2.0f, 0.0f });
		light4.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		light4.SetIntensity(1.0f);
		light4.SetActive(true);

		graphics.AddLight( { light1, light0, light2, light3, light4 } );

		mesh001 = std::make_shared<Mesh>("Assets/sphere.txt");
		mesh001->SetPosition(0.0f, 0.0f, 14.0f);
		mesh001->Scale(5.0f, 5.0f, 5.0f);

		mesh002 = std::make_shared<Mesh>("Assets/sphere.txt");
		mesh002->SetPosition(-15.0f, 0.0f, 14.0f);
		mesh002->Scale(5.0f, 5.0f, 5.0f);

		mesh003 = std::make_shared<Mesh>("Assets/cube.txt");
		mesh003->SetPosition(0.0f, -10.0f, 0.0f);
		mesh003->Scale(300.0f, 0.0f, 300.0f);

		mesh004 = std::make_shared<Mesh>("Assets/sphere.txt");
		mesh004->SetPosition(-30.0f, 0.0f, 14.0f);
		mesh004->Scale(5.0f, 5.0f, 5.0f);

		mesh005 = std::make_shared<Mesh>("Assets/sphere.txt");
		mesh005->SetPosition(15.0f, 0.0f, 14.0f);
		mesh005->Scale(5.0f, 5.0f, 5.0f);

		sky_dome = std::make_unique<Skydome>("Assets/skysphere.txt");

		// INSTANCE SOME RANDOM MESHES ALL AROUND BUT WITHIN PLANE
		/*random_meshes = std::vector<std::shared_ptr<Mesh>>(20);

		for (int i = 0; i < random_meshes.size(); i++)
		{
			// CALL CTOR
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("cube.txt");

			// SET RANDOM POS
			// x pos is given by: pos + width && pos - width/2
			float x = RandomFloat(mesh003->GetPosition().x - 200.0f / 2, mesh003->GetPosition().x + 200.0f / 2);
			float y = RandomFloat(mesh003->GetPosition().y, 20.0f);
			float z = RandomFloat(mesh003->GetPosition().z - 200.0f / 2, mesh003->GetPosition().z + 200.0f / 2);

			mesh->SetPosition(x, y, z);
			mesh->Scale(5.0f, 5.0f, 5.0f);
			mesh->Rotate(x, y, z);

			random_meshes[i] = mesh;
		}*/

		tex0 = std::make_shared<Texture>("Assets/Abstract_albedo.jpg");
		tex1 = std::make_shared<Texture>("Assets/Abstract_normal.jpg");

		tex2 = std::make_shared<Texture>("Assets/organic_albedo.jpg");
		tex3 = std::make_shared<Texture>("Assets/organic_normal.jpg");
	}

	void Load(winrt::hstring const&)
	{

	}

	void Run()
	{
		Graphics& graph = Graphics::Singleton();

		std::shared_ptr<MaterialProperties> metal_material = std::make_shared<MaterialProperties>();
		metal_material->material.Emissive = DirectX::XMFLOAT4(0.1, 0.1, 0.1, 1.0);
		metal_material->material.Ambient = DirectX::XMFLOAT4(0.1, 0.1, 0.1, 1.0);
		metal_material->material.Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		metal_material->material.Specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		metal_material->material.SpecularPower = 51.8f;
		metal_material->material.UseTexture = 1;	// albedo
		metal_material->material.UseNormal = 1;	// normal map

		std::shared_ptr<MaterialProperties> pearl_material = std::make_shared<MaterialProperties>();
		pearl_material->material.Ambient = DirectX::XMFLOAT4(0, 0, 0, 1.0);
		pearl_material->material.Diffuse = DirectX::XMFLOAT4(0.5, 0, 0, 1.0);
		pearl_material->material.Specular = DirectX::XMFLOAT4(0.7, 0.6, 0.6, 1.0);
		pearl_material->material.SpecularPower = 32.0f;
		pearl_material->material.UseTexture = 1;	// albedo
		pearl_material->material.UseNormal = 1;	// normal map

		std::shared_ptr<MaterialProperties> emerald_material = std::make_shared<MaterialProperties>();
		emerald_material->material.Ambient = DirectX::XMFLOAT4(0.0215, 0.1745, 0.0215, 1.0);
		emerald_material->material.Diffuse = DirectX::XMFLOAT4(0.07568, 0.61424, 0.07568, 1.0);
		emerald_material->material.Specular = DirectX::XMFLOAT4(0.633, 0.727811, 0.633, 1.0);
		emerald_material->material.SpecularPower = 76.8f;
		emerald_material->material.UseTexture = 0;	// albedo
		emerald_material->material.UseNormal = 0;	// normal map

		std::shared_ptr<MaterialProperties> obsidian_material = std::make_shared<MaterialProperties>();
		obsidian_material->material.Ambient = DirectX::XMFLOAT4(0.05375, 0.05, 0.06625, 1.0);
		obsidian_material->material.Diffuse = DirectX::XMFLOAT4(0.18275, 0.17, 0.22525, 1.0);
		obsidian_material->material.Specular = DirectX::XMFLOAT4(0.332741, 0.328634, 0.346435, 1.0);
		obsidian_material->material.SpecularPower = 38.4f;
		obsidian_material->material.UseTexture = 0;	// albedo
		obsidian_material->material.UseNormal = 0;	// normal map

		for(;;)
		{
			CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			Graphics::Singleton().Clear();

			StartCounter();
			double time = GetCounter();
			time += 1.0f / 60.0f;

#pragma region INPUT

			if (CoreWindow::GetForCurrentThread().GetAsyncKeyState(VirtualKey::D) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Translate(DirectX::XMVectorMultiply(Graphics::Singleton().GetMainCamera()->GetRightVector(), DirectX::XMVectorSet(30.0f * time, 30.0f * time, 30.0f * time, 30.0f * time)));
			}

			if (CoreWindow::GetForCurrentThread().GetKeyState(VirtualKey::A) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Translate(DirectX::XMVectorMultiply(Graphics::Singleton().GetMainCamera()->GetLeftVector(), DirectX::XMVectorSet(30.0f * time, 30.0f * time, 30.0f * time, 30.0f * time)));
			}

			if (CoreWindow::GetForCurrentThread().GetKeyState(VirtualKey::W) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Translate(DirectX::XMVectorMultiply(Graphics::Singleton().GetMainCamera()->GetForwardVector(), DirectX::XMVectorSet(30.0f * time, 30.0f * time, 30.0f * time, 30.0f * time)));
			}

			if (CoreWindow::GetForCurrentThread().GetKeyState(VirtualKey::S) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Translate(DirectX::XMVectorMultiply(Graphics::Singleton().GetMainCamera()->GetBackwardVector(), DirectX::XMVectorSet(30.0f * time, 30.0f * time, 30.0f * time, 30.0f * time)));
			}

			if (CoreWindow::GetForCurrentThread().GetKeyState(VirtualKey::Q) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Translate(DirectX::XMVectorMultiply(Graphics::Singleton().GetMainCamera()->GetUpVector(), DirectX::XMVectorSet(30.0f * time, 30.0f * time, 30.0f * time, 30.0f * time)));
			}

			if (CoreWindow::GetForCurrentThread().GetKeyState(VirtualKey::E) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Translate(DirectX::XMVectorMultiply(Graphics::Singleton().GetMainCamera()->GetDownVector(), DirectX::XMVectorSet(30.0f * time, 30.0f * time, 30.0f * time, 30.0f * time)));
			}

			// ROTATION
			if (CoreWindow::GetForCurrentThread().GetKeyState(VirtualKey::Right) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Rotate(0.0f, 1.0f * time, 0.0f);
			}

			if (CoreWindow::GetForCurrentThread().GetKeyState(VirtualKey::Left) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Rotate(0.0f, -1.0f * time, 0.0f);
			}

			if (CoreWindow::GetForCurrentThread().GetKeyState(VirtualKey::Up) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Rotate(1.0f * time, 0.0f, 0.0f);
			}

			if (CoreWindow::GetForCurrentThread().GetKeyState(VirtualKey::Down) == CoreVirtualKeyStates::Down)
			{
				Graphics::Singleton().GetMainCamera()->Rotate(-1.0f * time, 0.0f, 0.0f);
			}

			if (CoreWindow::GetForCurrentThread().GetAsyncKeyState(VirtualKey::K) == CoreVirtualKeyStates::Down)
			{
				metal_material->material.Specular = DirectX::XMFLOAT4(0, 0, 0, 1.0f);
			}

			if (CoreWindow::GetForCurrentThread().GetAsyncKeyState(VirtualKey::L) == CoreVirtualKeyStates::Down)
			{
				metal_material->material.Specular = DirectX::XMFLOAT4(1, 1, 1, 1.0f);
			}

#pragma endregion

			// SKYDOME
			sky_dome->Render();

			mesh001->Draw(tex0, tex1, metal_material);
			mesh001->Rotate(0.01f, 0.0f, 0.01f);

			mesh002->Draw(tex2, tex3, pearl_material);
			mesh002->Rotate(0.01f, 0.0f, 0.01f);

			mesh003->Draw(tex0, tex1, obsidian_material);

			mesh004->Draw(nullptr, nullptr, emerald_material);
			mesh004->Rotate(0.01f, 0.0f, 0.01f);

			mesh005->Draw(nullptr, nullptr, obsidian_material);
			mesh005->Rotate(0.01f, 0.0f, 0.01f);

			// Present swap chain scene
			Graphics::Singleton().Present();
		}
	}

private:

	// DirectX
	std::shared_ptr<Mesh>			mesh001;
	std::shared_ptr<Mesh>			mesh002;
	std::shared_ptr<Mesh>			mesh003;
	std::shared_ptr<Mesh>			mesh004;
	std::shared_ptr<Mesh>			mesh005;
	std::vector<std::shared_ptr<Mesh>> random_meshes;

	std::unique_ptr<Skydome> sky_dome;

	std::shared_ptr<GPUConstBuffer> light_buffer;
	std::shared_ptr<GPUConstBuffer> cam_buffer;
	std::shared_ptr<GPUConstBuffer> material_buffer;

	std::shared_ptr<Texture> tex0;
	std::shared_ptr<Texture> tex1;

	std::shared_ptr<Texture> tex2;
	std::shared_ptr<Texture> tex3;

	winrt::Windows::Foundation::Rect rect;
};

class ViewProviderFactory : public winrt::implements<ViewProviderFactory, IFrameworkViewSource>
{
public:
	IFrameworkView CreateView()
	{
		return winrt::make<ViewProvider>();
	}
};