#include "LightObject.h"
#include "LightSpot.h"
#include "Transform.h"
#include "Model.h"
#include "Shader.h"
#include "UserControl.h"
#include "Camera.h"
#include "CameraComponent.h"

LightObject::LightObject(std::string modelName, std::string textureName, Vector3 color)
{
	this->modelName = modelName;
	this->textureName = textureName;
	this->color = color;
}
LightObject::~LightObject() {}

void LightObject::Awake()
{
	AddComponent<Transform>();
}

void LightObject::Start()
{
	shader->Load(ASSET_PATH + "shaders//light", VertShader | FragShader);
	texture->Load(ASSET_PATH + textureName);
	model->Load(ASSET_PATH + "models//" + modelName);
}


void LightObject::Update() {}

void LightObject::RenderForward()
{
	texture->Bind(0);

	GameObjectManager* goMan = GameObjectManager::GetInstance();
	CameraComponent* cameraComp = goMan->GetGameObjectTag<Camera>("MainCamera")->GetComponent<CameraComponent>();

	glm::mat4 m = GetComponent<Transform>()->GetModelMatrix();
	glm::mat4 v = cameraComp->GetView();
	glm::mat4 p = cameraComp->GetProjection();


	shader->Bind();
	shader->SetInt("sampler", 0);
	shader->SetMat4("m", m);
	shader->SetMat4("v", v);
	shader->SetMat4("p", p);
	shader->SetVec3("colour", color);
	model->Draw();
}
