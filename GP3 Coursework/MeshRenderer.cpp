#include "pch.h"
#include "Common.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() {}
MeshRenderer::~MeshRenderer()
{
	//Todo: release model and material
}

void MeshRenderer::OnAttach() {}
void MeshRenderer::OnCollide(RigidBody* other) {}
void MeshRenderer::Start() {}
void MeshRenderer::Update() {}

void MeshRenderer::Load(std::string modelName)
{
	model->Load(ASSET_PATH + "models//" + modelName);
}

void MeshRenderer::Load(std::string modelName, std::string mapName, char mapFlags)
{
	Load(modelName);
	material->Load(mapName, mapFlags);
}

void MeshRenderer::Load(std::string modelName, std::string mapName, char mapFlags, glm::vec2 textureScale)
{
	Load(modelName);
	material->Load(mapName, mapFlags, textureScale);
}

void MeshRenderer::Draw(Shader* deferredGeometry)
{
	material->Bind(deferredGeometry);

	glm::mat4 m = gameObject->GetComponent<Transform>()->GetModelMatrix();
	deferredGeometry->SetMat4("model", m);
	
	model->Draw();
}
