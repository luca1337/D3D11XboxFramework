#include "Engine.h"

#include "Adapter.h"
#include "Device.h"

Engine::Engine()
{
	adapter = std::make_shared<Adapter>();
	device = std::make_shared<Device>(adapter);
}

Engine& Engine::Singleton()
{
	static Engine* engine = nullptr;

	if (!engine)
	{
		engine = new Engine();
	}

	return *engine;
}

std::shared_ptr<Adapter> Engine::GetDxAdapter() const
{
	return adapter;
}

std::shared_ptr<Device> Engine::GetDxDevice() const
{
	return device;
}