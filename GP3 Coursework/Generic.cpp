#include "pch.h"
#include "common.h"
#include "Generic.h"

#include "MeshRenderer.h"
#include "Camera.h"
#include "CameraComponent.h"


Generic::Generic()
{
	invisible = true;
}

Generic::Generic(std::string modelName)
{
	this->modelName = modelName;
}

Generic::Generic(std::string modelName, Vector3 baseColor)
{
	this->modelName = modelName;
	this->baseColor = baseColor;
}

Generic::Generic(std::string modelName, Vector3 baseColor, float roughness, float metalness) : Generic(modelName, baseColor)
{
	this->roughness = roughness;
	this->metalness = metalness;
}

Generic::Generic(std::string modelName, std::string mapName, char mapFlags)
{
	this->modelName = modelName;
	this->mapName = mapName;
	this->mapFlags = mapFlags;
}

Generic::Generic(std::string modelName, std::string mapName, char mapFlags, glm::vec2 textureScale) : Generic(modelName, mapName, mapFlags)
{
	this->textureScale = textureScale;
}

Generic::~Generic() {}


void Generic::Awake()
{
	AddComponent<Transform>();
	if(!invisible)
		AddComponent<MeshRenderer>();
}

void Generic::Start()
{
	if (!invisible)
		GetComponent<MeshRenderer>()->Load(modelName, mapName, mapFlags, textureScale);
}

void Generic::Update() {}
void Generic::OnCollide(RigidBody* other) {}
void Generic::RenderForward() {}
