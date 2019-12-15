#include "pch.h"
#include "Resources.h"

//Please note!
//This resource managers was created and testing to fulfill the GP3 Coursework requirments,
//but it is not used. The class AssetManager is used instead.

Resources::Resources() {}
Resources* Resources::m_instance = nullptr;
Resources* Resources::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new Resources();

	return m_instance;
}
void Resources::AddShader(const std::string& directory, char shaderTags)
{
	if (m_shaders.find(directory) == m_shaders.end())
	{
		m_shaders[directory] = std::make_shared<Shader>();
		m_shaders[directory]->Load(ASSET_PATH + directory, shaderTags);
		std::cout << "Shader Loaded from " + directory << std::endl;
	}
}
void Resources::AddModel(const std::string& directory)
{
	if (m_models.find(directory) == m_models.end())
	{
		m_models[directory] = std::make_shared<Model>();
		m_models[directory]->Load(ASSET_PATH + directory);
		std::cout << "Model Loaded from " + directory << std::endl;
	}
}
void Resources::AddTexture(const std::string& directory)
{
	if (m_textures.find(directory) == m_textures.end())
	{
		m_textures[directory] = std::make_shared<Texture>();
		m_textures[directory]->Load(ASSET_PATH + directory);
		std::cout << "Texture Loaded from " + directory << std::endl;
	}
}
std::shared_ptr<Shader> Resources::GetShader(const std::string& name)
{
	return m_shaders[name];
}
std::shared_ptr<Model> Resources::GetModel(const std::string& name)
{
	return m_models[name];
}
std::shared_ptr<Texture> Resources::GetTexture(const std::string& name)
{
	return m_textures[name];
}
void Resources::ReleaseResources()
{
	for (auto iter = m_shaders.begin(); iter != m_shaders.end(); )
	{
		iter->second.reset();
		iter = m_shaders.erase(iter);
	}

	for (auto iter = m_models.begin(); iter != m_models.end(); )
	{
		iter->second.reset();
		iter = m_models.erase(iter);
	}

	for (auto iter = m_textures.begin(); iter != m_textures.end(); )
	{
		iter->second.reset();
		iter = m_textures.erase(iter);
	}
}

void Resources::ReleaseUnusedResourses()
{
	for (auto iter = m_shaders.begin(); iter != m_shaders.end(); )
		if (iter->second.use_count() == 1)
		{
			iter->second.reset();
			iter = m_shaders.erase(iter);
		}
		else
			iter++;

	for (auto iter = m_models.begin(); iter != m_models.end(); )
		if (iter->second.use_count() == 1)
		{
			iter->second.reset();
			iter = m_models.erase(iter);
		}
		else
			iter++;

	for (auto iter = m_textures.begin(); iter != m_textures.end(); )
		if (iter->second.use_count() == 1)
		{
			iter->second.reset();
			iter = m_textures.erase(iter);
		}
		else
			iter++;
}

void Resources::Quit()
{
	ReleaseResources();

	m_instance = nullptr;
	delete this;
}
