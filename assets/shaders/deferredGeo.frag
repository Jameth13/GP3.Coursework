#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMaps;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 T, B, N;
in vec3 Normal;

uniform vec3 baseColor;
uniform int hasAlbedoMap;
uniform int hasNormalMap;
uniform int hasRoughMap;
uniform int hasMetalMap;
uniform int hasAOMap;

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texRough;
uniform sampler2D texMetal;
uniform sampler2D texAO;

uniform float roughness;
uniform float metalness;

void main()
{
    //gBuffer positions
    gPosition = FragPos;
	
    //gBuffer normals
	if (hasNormalMap > 0)
	{
		vec3 tempNorm = normalize(texture(texNormal, TexCoords).rgb * 2.0 - vec3(1.0));
		gNormal = tempNorm.x * T + tempNorm.y * B + tempNorm.z * N;
	}
	else
		gNormal = normalize(Normal);
		
    //gBuffer albedo
	if (hasAlbedoMap > 0)
		gAlbedo = texture(texAlbedo, TexCoords).rgb * baseColor;
	else
		gAlbedo = baseColor;

    //Store map data
	if (hasRoughMap > 0)
		gMaps.r = texture(texRough, TexCoords).r;
	else
		gMaps.r = roughness;
	
	if (hasMetalMap > 0)
		gMaps.g = texture(texMetal, TexCoords).r;
	else
		gMaps.g = metalness;

	if (hasAOMap > 0)
		gMaps.b = texture(texAO, TexCoords).r;
	else
		gMaps.b = 1.0f;
}
