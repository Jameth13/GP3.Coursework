#version 330

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D diffuse;

void main()
{
	FragColor = texture2D(diffuse, texCoord);
}
