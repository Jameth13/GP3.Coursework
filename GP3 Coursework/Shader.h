#pragma once
#include "pch.h"
#include "common.h"
#include "GameObjectManager.h"
#include "transform.h"


class Shader
{
public:
	Shader();
	~Shader();

	void Load(const std::string& filename, char shaders);
	void Bind();

	void SetInt(std::string name, int value);
	void SetFloat(std::string name, float value);
	void SetVec2(std::string name, glm::vec2 value);
	void SetVec3(std::string name, glm::vec3 value);
	void SetVec4(std::string name, glm::vec4 value);
	void SetMat4(std::string name, glm::mat4 mat);

private:
	GameObjectManager* goMan = GameObjectManager::GetInstance();

	static GLint m_boundProgram;

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

	static const unsigned int maxShaders = 6;
	GLuint m_program;
	GLuint shaders[maxShaders];
};
