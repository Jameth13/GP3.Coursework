#pragma once
#include "pch.h"
#include "common.h"
#include "MeshRenderer.h"
#include "GameObjectManager.h"
#include "Shader.h"
#include "Texture.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"

//True type font Character
struct Character {
	GLuint TextureID;	// ID handle of the glyph texture
	glm::ivec2 Size;	// Size of glyph
	glm::ivec2 Bearing;	// Offset from baseline to left/top of glyph
	GLuint Advance;		// Horizontal offset to advance to next glyph
};

class Renderer
{
public:
	~Renderer();
	static Renderer* GetInstance();

	void Init(int displayWidth, int displayHeight);
	void InitBuffers();
	void InitShaders();
	void InitSkybox(std::string skyboxFolder);
	void Render();
	void RenderText(std::string text, glm::vec3 posScale, glm::vec3 color);

	float exposure = 1.2;
	float gamma = 2.2f;

	void RenderQuad();

	void Quit();


private:
	Renderer();
	static Renderer* instance;

	GameObjectManager* goMan = GameObjectManager::GetInstance();

	std::vector<MeshRenderer*> meshRenderers;

	int displayWidth, displayHeight = 1024;

	void RenderDeferred(Shader* shader);


	//True type font
	std::map<GLchar, Character> Characters;
	GLuint fontVAO, fontVBO;
	Shader* shaderFont = new Shader();


	//Skybox
	Shader* shaderSkybox = new Shader();
	Texture* texSkybox = new Texture();
	GLuint skyboxVAO, skyboxVBO;
	GLuint skyboxCubemap;
	void RenderSkybox();
	bool hasSkybox = false;


	//Deferred Rendering
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedo, gMaps;

	Shader* shaderDeferredGeometry = new Shader();
	Shader* shaderDeferredLighting = new Shader();

	void DeferredGeometry(Shader* shader, GLuint buffer);
	void DeferredLighting(Shader* shader, GLuint buffer);

	unsigned int quadVAO = 0;
	unsigned int quadVBO;


	//Shadow Mapping
	//Max lights should match definitions indeferredLighting.frag!
	const static unsigned int MAX_LIGHTS_DIR = 1;
	const static unsigned int MAX_LIGHTS_POINT = 6;
	const static unsigned int MAX_LIGHTS_SPOT = 2;
	const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
	//Directional light
	GLuint depthMapBuffer;	//Todo: Allow more directional lights.
	GLuint depthMap;
	Shader* shaderDepthMap = new Shader();
	void RenderDepthMap(Shader* shader, GLuint buffer, LightDirectional* light, int lightIndex);
	glm::mat4 lightSpaceMatrices[MAX_LIGHTS_DIR];

	//Cube maps for point and spot lights
	Shader* shaderDepthCubemap = new Shader();
	void RenderDepthCubemap(Shader* shader, GLuint buffer, LightPoint* light);
	void RenderDepthCubemap(Shader* shader, GLuint buffer, LightSpot* light);
	void RenderDepthCubemap(Shader* shader, GLuint buffer, glm::vec3 lightPos, glm::vec3 lightBrightness);
	GLuint depthCubemapBuffer[MAX_LIGHTS_POINT + MAX_LIGHTS_SPOT];
	GLuint depthCubemap[MAX_LIGHTS_POINT + MAX_LIGHTS_SPOT];


	//HDR
	unsigned int renderBuffer;
	unsigned int renderColor;
	Shader* shaderHDR = new Shader();
	unsigned int hdrBuffer;
	unsigned int hdrColor, hdrBright;
	//Ping pong
	unsigned int blurBuffers[2];
	unsigned int blurColor[2];
	//Blur and bloom
	Shader* shaderBlur = new Shader();
	Shader* shaderPost = new Shader();
};
