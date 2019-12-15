#include "pch.h"
#include "Shader.h"
#include "Camera.h"
#include "CameraComponent.h"


GLint Shader::m_boundProgram = 0;


Shader::Shader() {}

Shader::~Shader()
{
	for (unsigned int i = 0; i < maxShaders; i++)
	{
		glDetachShader(m_program, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	glDeleteProgram(m_program);
}


void Shader::Bind()
{
	//Bind only if not already bound.
	glGetIntegerv(GL_CURRENT_PROGRAM, &m_boundProgram);

	if (m_boundProgram != m_program)
		glUseProgram(m_program);
}


void Shader::SetInt(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
}

void Shader::SetFloat(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
}

void Shader::SetVec2(std::string name, glm::vec2 value)
{
	glUniform2fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(std::string name, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(std::string name, glm::vec4 value)
{
	glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
}

void Shader::SetMat4(std::string name, glm::mat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::Load(const std::string& filename, char shaderTags)
{
	m_program = glCreateProgram();

	//Load shaders based on tags
	unsigned int shaderCount = 0;
	if (shaderTags & VertShader)
	{
		shaders[shaderCount] = CreateShader(LoadShader(filename + ".vert"), GL_VERTEX_SHADER);
		shaderCount++;
	}
	if (shaderTags & TescShader)
	{
		shaders[shaderCount] = CreateShader(LoadShader(filename + ".tesc"), GL_TESS_CONTROL_SHADER);
		shaderCount++;
	}
	if (shaderTags & TeseShader)
	{
		shaders[shaderCount] = CreateShader(LoadShader(filename + ".tese"), GL_TESS_EVALUATION_SHADER);
		shaderCount++;
	}
	if (shaderTags & GeomShader)
	{
		shaders[shaderCount] = CreateShader(LoadShader(filename + ".geom"), GL_GEOMETRY_SHADER);
		shaderCount++;
	}
	if (shaderTags & FragShader)
	{
		shaders[shaderCount] = CreateShader(LoadShader(filename + ".frag"), GL_FRAGMENT_SHADER);
		shaderCount++;
	}
	if (shaderTags & CompShader)
	{
		shaders[shaderCount] = CreateShader(LoadShader(filename + ".comp"), GL_COMPUTE_SHADER);
		shaderCount++;
	}

	for (unsigned int i = 0; i < shaderCount; i++)
		glAttachShader(m_program, shaders[i]); //add all our shaders to the shader program "shaders" 

	glLinkProgram(m_program); //create executables that will run on the GPU shaders
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(m_program); //check the entire program is valid
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}
