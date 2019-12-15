#version 330 core

in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec3 colour;

uniform sampler2D sampler;

void main()
{
	BrightColor = vec4(0);
	FragColor = vec4(1.1, 1.1, 1.1, 1.0) * texture2D(sampler, TexCoord) * vec4(colour, 1.0);
}
