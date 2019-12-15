#include "Material.h"

Material::Material() {}

Material::~Material()
{
	//Todo: Delete textures.
}

void Material::Load(Vector3 baseColor)
{
	this->baseColor = baseColor;
}

void Material::Load(Vector3 baseColor, float roughness, float metalness)
{
	this->baseColor = baseColor;
	this->roughness = roughness;
	this->metalness = metalness;
}
void Material::Load(std::string mapName, char mapFlags)
{
	this->mapFlags = mapFlags;

	if (mapFlags & AlbedoMap)
	{
		texAlbedo = new Texture();
		texAlbedo->Load(ASSET_PATH + "materials//" + mapName + "_albedo.jpg");
	}
	if (mapFlags & NormalMap)
	{
		texNormal = new Texture();
		texNormal->Load(ASSET_PATH + "materials//" + mapName + "_norm.jpg");
	}
	if (mapFlags & RoughMap)
	{
		texRough = new Texture();
		texRough->Load(ASSET_PATH + "materials//" + mapName + "_rough.jpg");
	}
	if (mapFlags & MetalMap)
	{
		texMetal = new Texture();
		texMetal->Load(ASSET_PATH + "materials//" + mapName + "_metal.jpg");
	}
	if (mapFlags & AOMap)
	{
		texAO = new Texture();
		texAO->Load(ASSET_PATH + "materials//" + mapName + "_ao.jpg");
	}
}

void Material::Load(std::string mapName, char mapFlags, glm::vec2 textureScale)
{
	Load(mapName, mapFlags);
	this->textureScale = textureScale;
}


void Material::SetColor(Vector3 baseColor)
{
	this->baseColor = baseColor;
}

void Material::Bind(Shader* deferredGeometry)
{
	//Texture positions.
	deferredGeometry->SetInt("texAlbedo", 0);
	deferredGeometry->SetInt("texNormal", 1);
	deferredGeometry->SetInt("texRough", 2);
	deferredGeometry->SetInt("texMetal", 3);
	deferredGeometry->SetInt("texAO", 4);

	//Bind textures or set valuess
	if (mapFlags & AlbedoMap)
		texAlbedo->Bind(0);

	if (mapFlags & NormalMap)
		texNormal->Bind(1);

	if (mapFlags & RoughMap)
		texRough->Bind(2);
	else
		deferredGeometry->SetFloat("roughness", roughness);

	if (mapFlags & MetalMap)
		texMetal->Bind(3);
	else
		deferredGeometry->SetFloat("metalness", metalness);

	if (mapFlags & AOMap)
		texAO->Bind(4);


	//Set available maps
	deferredGeometry->SetInt("hasAlbedoMap", mapFlags & AlbedoMap);
	deferredGeometry->SetInt("hasNormalMap", mapFlags & NormalMap);
	deferredGeometry->SetInt("hasRoughMap",  mapFlags & RoughMap);
	deferredGeometry->SetInt("hasMetalMap",  mapFlags & MetalMap);
	deferredGeometry->SetInt("hasAOMap",     mapFlags & AOMap);

	//Base color
	deferredGeometry->SetVec3("baseColor", baseColor);
	//Texture scale
	deferredGeometry->SetVec2("textureScale", textureScale);
}
