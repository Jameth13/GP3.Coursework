#pragma once
#include "pch.h"
#include "common.h"
#include "GameObject.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"

class LightObject : public GameObject
{
public:
	LightObject(std::string modelName, std::string textureName, Vector3 color);
	~LightObject();

	void Awake();
	void Start();
	void Update();

	void RenderForward();

	Vector3 color = Vector3(1.2f);

private:
	Texture* texture = new Texture();
	Model* model = new Model();
	Shader* shader = new Shader();

	std::string modelName, textureName = "";
};
