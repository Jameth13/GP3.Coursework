#version 330
layout (location = 0) in vec3 positions;
layout (location = 1) in vec4 colours;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 normals;

out vec2 texCoord;

uniform mat4 MVP;

void main()
{
	texCoord = texCoords;
	gl_Position = MVP * vec4(positions, 1.0);
}
