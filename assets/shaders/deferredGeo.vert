#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 T, B, N;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int hasNormalMap;

uniform vec2 textureScale;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
	
	TexCoords = aTexCoords * textureScale;

	mat3 normalMatrix = transpose(inverse(mat3(model)));

	if (hasNormalMap > 0)
	{
		//Has normal map
		T = normalize(normalMatrix * aTangent);
		B = normalize(normalMatrix * aBitangent);
		N = normalize(normalMatrix * aNormal);
	}
	else
	{
		//No normal map
		Normal = normalMatrix * aNormal;
	}
	 
    gl_Position = projection * view * worldPos;
}
