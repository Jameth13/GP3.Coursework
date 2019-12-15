#pragma once
#include "pch.h"
#include "common.h"
#include "Mesh.h"

class AssetManager
{
private:
	//Private constructor, this prevents creating an instance.
	AssetManager();

	//Static pointer
	static AssetManager* instance;

public:
	~AssetManager();

	std::map<std::string, GLuint> textures;
	std::map<std::string, std::vector<Mesh*>> meshes;
	std::map<std::string, unsigned int> sounds;
	//Todo: Shaders

	//Static method to get pointer
	static AssetManager* GetInstance();
};
