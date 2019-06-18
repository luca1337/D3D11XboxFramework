#pragma once

#include <memory>
#include <string>

class Mesh;
class VertexShader;
class PixelShader;

class Skydome
{
public:
	Skydome(const std::string& skydome_file);

	void Render();

private:
	std::shared_ptr<Mesh> skydome_mesh;

	std::shared_ptr<VertexShader> skydome_vs;
	std::shared_ptr<PixelShader> skydome_ps;

	std::shared_ptr<VertexShader> basic_vs;
	std::shared_ptr<PixelShader> basic_ps;
};