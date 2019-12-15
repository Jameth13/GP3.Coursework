#pragma once
#include "pch.h"
#include "Common.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
class Resources
{
public:
	static Resources* GetInstance();

	void AddShader(const std::string& directory, char shaderTags);
	void AddModel(const std::string& directory);
	void AddTexture(const std::string& directory);

	std::shared_ptr<Shader> GetShader(const std::string& name);
	std::shared_ptr<Model> GetModel(const std::string& name);
	std::shared_ptr<Texture> GetTexture(const std::string& name);

	void ReleaseResources();
	void ReleaseUnusedResourses();
	void Quit();

private:
	Resources();

	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
	std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	static Resources* m_instance;
};
