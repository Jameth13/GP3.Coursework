#pragma once
#include "GameObject.h"
#include "Shader.h"

class Generic: public GameObject
{
public:
	Generic();
	Generic(std::string modelName);
	Generic(std::string modelName, Vector3 baseColor);
	Generic(std::string modelName, Vector3 baseColor, float roughness, float metalness);
	Generic(std::string modelName, std::string mapName, char mapFlags);
	Generic(std::string modelName, std::string mapName, char mapFlags, glm::vec2 textureScale);
	~Generic();

	void Awake();
	void Start();
	void Update();
	void RenderForward();

private:
	char mapFlags;
	Vector3 baseColor = Vector3(0.8f);
	float roughness = 0.5f;
	float metalness = 0.5f;
	std::string modelName;
	std::string mapName;
	glm::vec2 textureScale = glm::vec2(1.0f, 1.0f);

	bool invisible = false;
};
