#include "Renderer.h"
#include "Camera.h"
#include "CameraComponent.h"

#include "InputManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H


Renderer* Renderer::instance = 0;

Renderer::Renderer() {}
Renderer::~Renderer() {}

Renderer* Renderer::GetInstance()
{
	if (instance == 0)
		instance = new Renderer();

	return instance;
}


void Renderer::Init(int displayWidth, int displayHeight)
{
	this->displayWidth = displayWidth;
	this->displayHeight = displayHeight;

	//Standard OpenGL settings.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	SDL_GL_SetSwapInterval(0);	//Disable vsync.
	//glFrontFace(GL_CW);		//Change winding-order.
}

void Renderer::InitBuffers()
{
	//True type font
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	std::string path = ASSET_PATH + "verdana.ttf";
	if (FT_New_Face(ft, path.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);		// Set size to load glyphs as
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Disable byte-alignment restriction

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure fontVAO/fontVBO for texture quads
	glGenVertexArrays(1, &fontVAO);
	glGenBuffers(1, &fontVBO);
	glBindVertexArray(fontVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//gBuffer framebuffer
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	//Position texture
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, displayWidth, displayHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	//Normal texture
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, displayWidth, displayHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	//Albedo texture
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, displayWidth, displayHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	//Maps texture
	glGenTextures(1, &gMaps);
	glBindTexture(GL_TEXTURE_2D, gMaps);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, displayWidth, displayHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMaps, 0);

	GLuint gAttachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, gAttachments);

	GLuint gDepth;
	glGenRenderbuffers(1, &gDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, displayWidth, displayHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepth);
	//Check is bGuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "gBuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	//Render framebuffer
	glGenFramebuffers(1, &renderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer);
	//PreHDR texture
	glGenTextures(1, &renderColor);
	glBindTexture(GL_TEXTURE_2D, renderColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, displayWidth, displayHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderColor, 0);
	GLuint hdrPreAttachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, hdrPreAttachments);
	
	//PreHDR depth buffer
	GLuint renderDepth;
	glGenRenderbuffers(1, &renderDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, renderDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, displayWidth, displayHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderDepth);

	//Check if PreHDR framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Render buffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	//HDR framebuffer
	glGenFramebuffers(1, &hdrBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrBuffer);
	//HDR colour texture
	glGenTextures(1, &hdrColor);
	glBindTexture(GL_TEXTURE_2D, hdrColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, displayWidth, displayHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrColor, 0);
	//HDR bright texture
	glGenTextures(1, &hdrBright);
	glBindTexture(GL_TEXTURE_2D, hdrBright);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, displayWidth, displayHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, hdrBright, 0);

	GLuint hdrAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, hdrAttachments);

	//HDR depth buffer
	GLuint hdrDepth;
	glGenRenderbuffers(1, &hdrDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, displayWidth, displayHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrDepth);

	//Check if HDR framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "HDR buffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	//Ping-pong framebuffers for blurring
	glGenFramebuffers(2, blurBuffers);
	glGenTextures(2, blurColor);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurBuffers[i]);
		glBindTexture(GL_TEXTURE_2D, blurColor[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, displayWidth, displayHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColor[i], 0);

		//Check if ping-pong framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Ping-pong buffer (" + std::to_string(i) + ") not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}



	//Depth Map framebuffer
	glGenFramebuffers(1, &depthMapBuffer);
	//Depth Map texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	//Depth Map depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//Check if Depth Map framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Depth Map buffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Depth Cubemap framebuffers for Point and Spot lights.
	for (unsigned int i = 0u; i < MAX_LIGHTS_POINT + MAX_LIGHTS_SPOT; i++)
	{
		glGenFramebuffers(1, &depthCubemapBuffer[i]);
		glGenTextures(1, &depthCubemap[i]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap[i]);
		for (unsigned int face = 0; face < 6; face++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//Depth Cubemap depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapBuffer[i]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		//Check if Depth Cubemap framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Depth Cubemap buffer (" + std::to_string(i) + ") not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void Renderer::InitShaders()
{
	//True type font
	shaderFont->Load(ASSET_PATH + "shaders//text", VertShader | FragShader); 
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(displayWidth), 0.0f, static_cast<GLfloat>(displayHeight));
	shaderFont->Bind();
	shaderFont->SetMat4("projection", projection);


	//Deferred Geometry shader
	shaderDeferredGeometry->Load(ASSET_PATH + "shaders//deferredGeo", VertShader | FragShader);
	shaderDeferredGeometry->Bind();
	shaderDeferredGeometry->SetInt("texAlbedo", 0);
	shaderDeferredGeometry->SetInt("texNormal", 1);
	shaderDeferredGeometry->SetInt("texRough", 2);
	shaderDeferredGeometry->SetVec2("textureScale", glm::vec2(1.0f));

	//Deferred Lighting shader
	shaderDeferredLighting->Load(ASSET_PATH + "shaders//deferredLighting", VertShader | FragShader);

	shaderDeferredLighting->Bind();
	shaderDeferredLighting->SetInt("gPosition", 0);
	shaderDeferredLighting->SetInt("gNormal", 1);
	shaderDeferredLighting->SetInt("gAlbedo", 2);
	shaderDeferredLighting->SetInt("gMaps", 3);
	shaderDeferredLighting->SetInt("depthMap", 4);

	for (unsigned int i = 0u; i < MAX_LIGHTS_POINT + MAX_LIGHTS_SPOT; i++)
		shaderDeferredLighting->SetInt("depthCubemap[" + std::to_string(i) + "]", 5 + i);


	//HDR shader
	shaderHDR->Load(ASSET_PATH + "shaders//hdr", VertShader | FragShader);
	shaderHDR->Bind();
	shaderHDR->SetInt("color", 0);


	//Blur and Post shaders
	shaderBlur->Load(ASSET_PATH + "shaders//blur", VertShader | FragShader);
	shaderBlur->Bind();
	shaderBlur->SetInt("image", 0);

	shaderPost->Load(ASSET_PATH + "shaders//post", VertShader | FragShader);
	shaderPost->Bind();
	shaderPost->SetInt("scene", 0);
	shaderPost->SetInt("bloomBlur", 1);
	shaderPost->SetFloat("exposure", exposure);
	shaderPost->SetFloat("gamma", gamma);


	//Depth Map shaders
	shaderDepthMap->Load(ASSET_PATH + "shaders//depthMap", VertShader | FragShader);
	shaderDepthCubemap->Load(ASSET_PATH + "shaders//depthCubemap", VertShader | GeomShader | FragShader);
}

void Renderer::InitSkybox(std::string skyboxFolder)
{
	hasSkybox = true;
	shaderSkybox->Load(ASSET_PATH + "shaders//skybox", VertShader | FragShader);

	std::vector<std::string> skyboxFaces
	{
		ASSET_PATH + skyboxFolder + "//right.jpg",
		ASSET_PATH + skyboxFolder + "//left.jpg",
		ASSET_PATH + skyboxFolder + "//top.jpg",
		ASSET_PATH + skyboxFolder + "//bottom.jpg",
		ASSET_PATH + skyboxFolder + "//front.jpg",
		ASSET_PATH + skyboxFolder + "//back.jpg"
	};
	skyboxCubemap= texSkybox->LoadCubemap(skyboxFaces);


	float skyboxVertices[] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}


void Renderer::Render()
{
	//Clear default buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	meshRenderers = goMan->GetComponents<MeshRenderer>();


	//Render depth to texture
	auto lightsDirectional = goMan->GetComponents<LightDirectional>();
	for (unsigned int i = 0u; lightsDirectional.size() && i < MAX_LIGHTS_DIR; i++)
		RenderDepthMap(shaderDepthMap, depthMapBuffer, lightsDirectional[i], i);

	auto lightsPoint = goMan->GetComponents<LightPoint>();
	for (unsigned int i = 0u; i < lightsPoint.size() && i < MAX_LIGHTS_POINT; i++)
		RenderDepthCubemap(shaderDepthCubemap, depthCubemapBuffer[i], lightsPoint[i]);

	auto lightsSpot = goMan->GetComponents<LightSpot>();
	for (unsigned int i = 0u; i < lightsSpot.size() && i < MAX_LIGHTS_SPOT; i++)
		RenderDepthCubemap(shaderDepthCubemap, depthCubemapBuffer[MAX_LIGHTS_POINT + i], lightsSpot[i]);


	//Deferred geometry and lighting pass
	DeferredGeometry(shaderDeferredGeometry, gBuffer);
	DeferredLighting(shaderDeferredLighting, renderBuffer);


	//Copy depth info from gBuffer to renderBuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderBuffer);
	glBlitFramebuffer(0, 0, displayWidth, displayHeight, 0, 0, displayWidth, displayHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	//Forward render to renderBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer);
	goMan->RenderForward();


	//Copy render colours over to hdrBuffer, split into hdrColor and hdrBright.
	glBindFramebuffer(GL_FRAMEBUFFER, hdrBuffer);
	shaderHDR->Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderColor);
	RenderQuad();


	//Copy depth info from renderBuffer to hdrBuffer.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, renderBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, hdrBuffer);
	glBlitFramebuffer(0, 0, displayWidth, displayHeight, 0, 0, displayWidth, displayHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);


	//Blur hdrBright
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 12;
	shaderBlur->Bind();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurBuffers[horizontal]);
		shaderBlur->SetInt("horizontal", horizontal);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? hdrBright : blurColor[!horizontal]);
		RenderQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	//Render skybox to hdrBuffer.
	if (hasSkybox)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, hdrBuffer);
		RenderSkybox();
	}

	//Post-processing
		//Combine hdrColor with blurred hdrBright.
		//Adjust according to exposure and gamma.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderPost->Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrColor);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, blurColor[!horizontal]);
	RenderQuad();
}


void Renderer::RenderText(std::string text, glm::vec3 posScale, glm::vec3 color)
{
	//OpenGL setttings for font
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate corresponding render state	
	shaderFont->Bind();
	shaderFont->SetVec3("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(fontVAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = posScale.x + ch.Bearing.x * posScale.z;
		GLfloat ypos = posScale.y - (ch.Size.y - ch.Bearing.y) * posScale.z;

		GLfloat w = ch.Size.x * posScale.z;
		GLfloat h = ch.Size.y * posScale.z;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		posScale.x += (ch.Advance >> 6) * posScale.z; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Renderer::RenderDeferred(Shader* shader)
{
	for (unsigned int i = 0u; i < meshRenderers.size(); i++)
		meshRenderers[i]->Draw(shader);
}


void Renderer::RenderSkybox()
{
	glDepthFunc(GL_LEQUAL);  //Set depth function to less than or equal
	shaderSkybox->Bind();
	shaderSkybox->SetInt("skybox", 0);
	GameObjectManager* goMan = GameObjectManager::GetInstance();
	Camera* camera = goMan->GetGameObjectTag<Camera>("MainCamera");

	glm::mat4 trans = camera->GetComponent<Transform>()->GetTranslationMatrix();
	glm::mat4 v = camera->GetComponent<CameraComponent>()->GetView();
	glm::mat4 p = camera->GetComponent<CameraComponent>()->GetProjection();

	shaderSkybox->SetMat4("trans", trans);
	shaderSkybox->SetMat4("view", v);
	shaderSkybox->SetMat4("projection", p);
	//Bind skybox cubmap and draw cube.
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); //Return depth function to default
}

void Renderer::DeferredGeometry(Shader* shader, GLuint buffer)
{
	//Bind and clear gBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera* camera = goMan->GetGameObjectTag<Camera>("MainCamera");

	glm::mat4 view = camera->GetComponent<CameraComponent>()->GetView();
	glm::mat4 projection = camera->GetComponent<CameraComponent>()->GetProjection();

	shader->Bind();
	shader->SetMat4("view", view);
	shader->SetMat4("projection", projection);

	RenderDeferred(shader);

	//Unbind gBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DeferredLighting(Shader* shader, GLuint buffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gMaps);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	for (unsigned int i = 0; i < MAX_LIGHTS_POINT; i++)
	{
			glActiveTexture(GL_TEXTURE5 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap[i]);
	}

	for (unsigned int i = 0; i < MAX_LIGHTS_SPOT; i++)
	{
		glActiveTexture(GL_TEXTURE5 + MAX_LIGHTS_POINT + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap[MAX_LIGHTS_POINT + i]);
	}

	GameObjectManager* goMan = GameObjectManager::GetInstance();
	Camera* camera = goMan->GetGameObjectTag<Camera>("MainCamera");

	glm::vec3 cameraPos = camera->GetComponent<Transform>()->GetPos();

	auto lightsDirectional = goMan->GetComponents<LightDirectional>();
	auto lightsPoint = goMan->GetComponents<LightPoint>();
	auto lightsSpot = goMan->GetComponents<LightSpot>();

	shader->SetVec3("cameraPosition", cameraPos);


	//Directional Lights
	for (unsigned int i = 0u; i < lightsDirectional.size(); i++)
	{
		shader->SetVec3("lightsDirectional[" + std::to_string(i) + "].direction", lightsDirectional[i]->gameObject->GetComponent<Transform>()->GetForward());
		shader->SetVec3("lightsDirectional[" + std::to_string(i) + "].color", lightsDirectional[i]->color);

		shader->SetFloat("lightsDirectional[" + std::to_string(i) + "].intensity", lightsDirectional[i]->intensity);

		shader->SetInt("lightsDirectional[" + std::to_string(i) + "].castShadows", lightsDirectional[i]->castShadows);

		//Light space matrix
		shader->SetMat4("lightSpaceMatrix", lightSpaceMatrices[i]);
	}
	shader->SetInt("numLightsDirectional", lightsDirectional.size());
	if (lightsDirectional.size() > MAX_LIGHTS_DIR)
		std::cout << "Error! Too many directional lights (" << std::to_string(lightsDirectional.size()) << " of " << std::to_string(MAX_LIGHTS_DIR) << ")." << std::endl;

	//Point Lights
	for (unsigned int i = 0u; i < lightsPoint.size() && i < MAX_LIGHTS_POINT; i++)
	{
		shader->SetVec3("lightsPoint[" + std::to_string(i) + "].position", lightsPoint[i]->gameObject->GetComponent<Transform>()->GetPos());
		shader->SetVec3("lightsPoint[" + std::to_string(i) + "].color", lightsPoint[i]->color);

		Vector3 brightness = lightsPoint[i]->GetBrightness();
		shader->SetFloat("lightsPoint[" + std::to_string(i) + "].intensity", brightness.x);
		shader->SetFloat("lightsPoint[" + std::to_string(i) + "].dropoff", brightness.y);
		shader->SetFloat("lightsPoint[" + std::to_string(i) + "].range", brightness.z);

		shader->SetInt("lightsPoint[" + std::to_string(i) + "].castShadows", lightsPoint[i]->castShadows);
	}
	shader->SetInt("numLightsPoint", lightsPoint.size());
	if (lightsPoint.size() > MAX_LIGHTS_POINT)
		std::cout << "Error! Too many point lights (" << std::to_string(lightsPoint.size()) << " of " << std::to_string(MAX_LIGHTS_POINT) << ")." << std::endl;

	//Spot Lights
	for (unsigned int i = 0u; i < lightsSpot.size() && i < MAX_LIGHTS_SPOT; i++)
	{
		shader->SetVec3("lightsSpot[" + std::to_string(i) + "].position", lightsSpot[i]->gameObject->GetComponent<Transform>()->GetPos());
		shader->SetVec3("lightsSpot[" + std::to_string(i) + "].direction", lightsSpot[i]->gameObject->GetComponent<Transform>()->GetForward());
		shader->SetVec3("lightsSpot[" + std::to_string(i) + "].color", lightsSpot[i]->color);

		Vector3 brightness = lightsSpot[i]->GetBrightness();
		glm::vec2 angles = lightsSpot[i]->GetAngle();
		shader->SetFloat("lightsSpot[" + std::to_string(i) + "].intensity", brightness.x);
		shader->SetFloat("lightsSpot[" + std::to_string(i) + "].dropoff", brightness.y);
		shader->SetFloat("lightsSpot[" + std::to_string(i) + "].range", brightness.z);

		shader->SetFloat("lightsSpot[" + std::to_string(i) + "].innerAngle", glm::cos(glm::radians(angles.x)));
		shader->SetFloat("lightsSpot[" + std::to_string(i) + "].outerAngle", glm::cos(glm::radians(angles.y)));

		shader->SetInt("lightsSpot[" + std::to_string(i) + "].castShadows", lightsSpot[i]->castShadows);
	}
	shader->SetInt("numLightsSpot", lightsSpot.size());
	if (lightsSpot.size() > MAX_LIGHTS_SPOT)
		std::cout << "Error! Too many spot lights (" << std::to_string(lightsSpot.size()) << " of " << std::to_string(MAX_LIGHTS_SPOT) << ")." << std::endl;


	RenderQuad();
}

void Renderer::RenderQuad()
{
	//Setup quadVAO once.
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			//Positions        //Texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Renderer::Quit()
{
	//Delete buffers
	unsigned int buffers[] = { fontVBO, skyboxVBO, gBuffer, quadVBO, depthMapBuffer, renderBuffer, hdrBuffer };
	glDeleteBuffers(sizeof(buffers) / sizeof(buffers[0]), buffers);
	glDeleteBuffers(sizeof(depthCubemapBuffer) / sizeof(depthCubemapBuffer[0]), depthCubemapBuffer);
	glDeleteBuffers(sizeof(blurBuffers) / sizeof(blurBuffers[0]), blurBuffers);

	//Delete buffer textures
	unsigned int bufferTextures[] = { skyboxCubemap, gPosition, gNormal, gAlbedo, gMaps, depthMap, renderColor, hdrColor, hdrBright };
	glDeleteTextures(sizeof(bufferTextures) / sizeof(bufferTextures[0]), bufferTextures);
	glDeleteTextures(sizeof(depthCubemap) / sizeof(depthCubemap[0]), depthCubemap);
	glDeleteTextures(sizeof(blurColor) / sizeof(blurColor[0]), blurColor);

	//Delete vertext array objects
	unsigned int vaos[] = { fontVAO, skyboxVAO, quadVAO };
	glDeleteVertexArrays(sizeof(vaos) / sizeof(vaos[0]), vaos);

	//Delete shaders

	//Delete textures

	//Delete singleton
	instance = nullptr;
	delete this;
}

void Renderer::RenderDepthMap(Shader* shader, GLuint buffer, LightDirectional* light, int lightIndex)
{
	if (!light->castShadows)
		return;

	Vector3 shadowMapSizes = light->shadowMapSizes;
	glm::mat4 lightProjection = glm::ortho(-shadowMapSizes.x / 2, shadowMapSizes.x / 2, -shadowMapSizes.y / 2, shadowMapSizes.y / 2, 0.0f, shadowMapSizes.z);

	Vector3 lightLookAt = light->gameObject->GetComponent<Transform>()->GetPos();
	Vector3 lightDir = light->gameObject->GetComponent<Transform>()->GetForward();
	Vector3 lightPos = lightDir * -shadowMapSizes.z / 2;

	glm::mat4 lightView = glm::lookAt<float, glm::precision::highp>(lightPos, lightLookAt, glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrices[lightIndex] = lightProjection * lightView;


	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	shader->Bind();
	shader->SetMat4("lightSpaceMatrix", lightSpaceMatrices[lightIndex]);

	RenderDeferred(shader);

	//Reset viewport
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, displayWidth, displayHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderDepthCubemap(Shader* shader, GLuint buffer, LightPoint* light)
{
	if (!light->castShadows)
		return;

	Vector3 lightPos = light->gameObject->GetComponent<Transform>()->GetPos();
	Vector3 lightBrightness = light->GetBrightness();

	RenderDepthCubemap(shader, buffer, lightPos, lightBrightness);
}

void Renderer::RenderDepthCubemap(Shader* shader, GLuint buffer, LightSpot* light)
{
	if (!light->castShadows)
		return;

	Vector3 lightPos = light->gameObject->GetComponent<Transform>()->GetPos();
	Vector3 lightBrightness = light->GetBrightness();

	RenderDepthCubemap(shader, buffer, lightPos, lightBrightness);
}

void Renderer::RenderDepthCubemap(Shader* shader, GLuint buffer, glm::vec3 lightPos, glm::vec3 lightBrightness)
{
	float near_plane = 0.1f;
	float far_plane = lightBrightness.z;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	shader->Bind();
	for (unsigned int i = 0; i < 6; ++i)
		shader->SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	shader->SetFloat("far_plane", far_plane);
	shader->SetVec3("lightPos", lightPos);
	RenderDeferred(shader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Reset viewport
	glViewport(0, 0, displayWidth, displayHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
