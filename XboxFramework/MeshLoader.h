#pragma once

#include <string>

class Mesh;

class MeshLoader
{
public:
	static void Load(std::string file_path, Mesh& mesh);
};