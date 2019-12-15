#pragma once
#include "Mesh.h"
#include "Vertex.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Model.h"
#include "Material.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	~MeshRenderer();

	void Load(std::string modelName);
	void Load(std::string modelName, std::string mapName, char mapFlags);
	void Load(std::string modelName, std::string mapName, char mapFlags, glm::vec2 textureScale);

	void Draw(Shader* deferredGeometry);

	void OnAttach();
	void OnCollide(RigidBody* other);
	void Start();
	void Update();

private:
	Model* model = new Model();
	Material* material = new Material();
};
