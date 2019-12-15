#pragma once
#include "pch.h"
#include "Common.h"
#include "Shader.h"
#include "Texture.h"

class Material
{
public:
	Material();
	~Material();
	void Load(Vector3 baseColor);
	void Load(Vector3 baseColor, float roughness, float metalness);
	void Load(std::string mapName, char mapFlags);
	void Load(std::string mapName, char mapFlags, glm::vec2 textureScale);

	void SetColor(Vector3 baseColor);

	void Bind(Shader* deferredGeometry);

private:
	char mapFlags = 0;

	Texture* texAlbedo;
	Texture* texNormal;
	Texture* texRough;
	Texture* texMetal;
	Texture* texAO;

	Vector3 baseColor = Vector3(1.0f);
	float roughness = 0.5f;
	float metalness = 0.5f;
	glm::vec2 textureScale = glm::vec2(1.0f);
};
