#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    gl_Position = p * v * m * vec4(position, 1.0);
	TexCoord = texCoord;
}
