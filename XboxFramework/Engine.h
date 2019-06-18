#pragma once

#include <memory>

class Adapter;
class Device;

class Engine final
{
public:
	static Engine& Singleton();

	std::shared_ptr<Adapter> GetDxAdapter() const;
	std::shared_ptr<Device> GetDxDevice() const;

private:
	Engine();

	std::shared_ptr<Adapter> adapter;
	std::shared_ptr<Device> device;
};