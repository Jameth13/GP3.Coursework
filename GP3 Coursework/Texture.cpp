#include "pch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLint Texture::m_boundTexture = 0;

Texture::Texture() {}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::Load(const std::string& directory)
{
	//Flip textures for use in OpenGL
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data = NULL;

	if (assetMan->textures.count(directory) > 0)
	{
		m_texture = assetMan->textures[directory];
		//Return to skip null data check and init.
		return;
	}
	else
	{
		newTextDir = directory;
		data = stbi_load(directory.c_str(), &width, &height, &nrChannels, 0);
	}


	//Check if texture was loaded.
	if (data == NULL)
	{
		std::cout << "Failed to load texture \"" << directory << "\". Using missing texture." << std::endl;

		//Hard-coded missing texture.
		width = 16;
		height = 16;
		nrChannels = 3;
		data = new unsigned char[768]{
			  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,
			  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,

			255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,
			255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,

			  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,
			  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,

			255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,
			255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,

			  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,
			  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,

			255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,
			255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,

			  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,
			  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,

			255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,
			255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,		255,   0, 255,	255,   0, 255,		  0,   0,   0,	  0,   0,   0,
		};
	}

	Init(width, height, nrChannels, data);
}

void Texture::Init(int width, int height, int nrChannels, unsigned char* data)
{
	GLuint texture = 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	assetMan->textures[newTextDir] = texture;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	auto glChannels = GL_RGBA;
	switch (nrChannels)
	{
	case 3:
		glChannels = GL_RGB;
		break;
	case 4:
		glChannels = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, glChannels, width, height, 0, glChannels, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	m_texture = texture;
}

void Texture::Bind(unsigned int index)
{
	//Bind only if not already bound.
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &m_boundTexture);

	if (m_boundTexture != m_texture)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
}

GLuint Texture::LoadCubemap(std::vector<std::string> faces)
{

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	unsigned char* data = NULL;
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//Don't flip cube maps.
		stbi_set_flip_vertically_on_load(false);
		data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return m_texture;
}
