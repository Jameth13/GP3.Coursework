#include "pch.h"
#include "Mesh.h"

GLint Mesh::m_boundVAO = 0;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices)
{
	m_indices = indices;
	m_vertices = vertices;
	//generate and bind vao so that it keeps the current vbo and ebo and attribs
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
	//tell the VAO that 1 is the position element
	SetUpAttrib(0, 3, GL_FLOAT, 0);;
	//uv
	SetUpAttrib(1, 2, GL_FLOAT, sizeof(glm::vec3));
	//normals
	SetUpAttrib(2, 3, GL_FLOAT, sizeof(glm::vec3) + sizeof(glm::vec2));
	//tangent
	SetUpAttrib(3, 3, GL_FLOAT, sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3));
	//bitangent
	SetUpAttrib(4, 3, GL_FLOAT, sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec3));
}
void Mesh::SetUpAttrib(int index, int count, int type, size_t offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, count, type, GL_FALSE, sizeof(Vertex), (void**)offset);
}
void Mesh::Bind()
{
	//Bind only if not already bound.
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &m_boundVAO);

	if (m_boundVAO != m_vao)
		glBindVertexArray(m_vao);
}
