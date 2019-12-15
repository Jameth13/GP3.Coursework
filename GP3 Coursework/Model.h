#pragma once
#include "Mesh.h"
#include "AssetManager.h"

class Model
{
private:
	AssetManager* assetMan = AssetManager::GetInstance();

	std::vector<Mesh*> m_meshes;
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	std::string newMeshesDir;

	const char* missingModelData = nullptr;
	size_t missingModelDataLength = 0;
	std::string missingModelDataString = "";

public:
	Model();
	~Model();

	void Load(std::string directory);
	void Draw();

	std::vector<Mesh*> GetMeshes() { return m_meshes; };
};
