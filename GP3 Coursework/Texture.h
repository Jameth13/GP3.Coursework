#pragma once
#include "pch.h"
#include "Common.h"
#include "AssetManager.h"

class Texture
{
public:
	Texture();
	~Texture();

	void Load(const std::string& directory);
	GLuint LoadCubemap(std::vector<std::string> faces);

	void Bind(unsigned int index);

private:
	static GLint m_boundTexture;

	GLuint m_texture;
	void Init(int width, int height, int nrChannels, unsigned char* data);
	AssetManager* assetMan = AssetManager::GetInstance();
	std::string newTextDir = "";
};
