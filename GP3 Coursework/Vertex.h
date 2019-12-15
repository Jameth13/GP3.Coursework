#pragma once
#include "pch.h"
#include "Common.h"
struct Vertex
{
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec2 texture = glm::vec2(0, 0);
	glm::vec3 normal = glm::vec3(0, 0, 0);
	glm::vec3 tangent = glm::vec3(0, 0, 0);
	glm::vec3 bitangent = glm::vec3(0, 0, 0);

	Vertex() {}
	Vertex(glm::vec3 p)
	{
		pos = p;
	}
	Vertex(glm::vec3 p, glm::vec2 t) : Vertex(p)
	{
		texture = t;
	}
	Vertex(glm::vec3 p, glm::vec2 t, glm::vec3 n) : Vertex(p, t)
	{
		normal = n;
	}
	Vertex(glm::vec3 p, glm::vec2 t, glm::vec3 n, glm::vec3 tan, glm::vec3 bitan) : Vertex(p, t, n)
	{
		tangent = tan;
		bitangent = bitan;
	}
};
