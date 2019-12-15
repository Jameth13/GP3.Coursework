#include "pch.h"
#include "Common.h"
#include "Application.h"
#include "MeshRenderer.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"
#include "Physics.h"
#include "BoxShape.h"
#include "RigidBody.h"

//Font
#include <ft2build.h>
#include FT_FREETYPE_H

Application* Application::m_application = nullptr;
Application::Application()
{
}

void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError());
		exit(-1);
	}
	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//creating window
	m_window = SDL_CreateWindow("GP3 CW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);
	SDL_CaptureMouse(SDL_TRUE);
	OpenGlInit();
	GameInit();
}
void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();
	SDL_GL_SetSwapInterval(1);
	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);



	//Start: Font
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "..//..//..//assets//arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
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
	//End: Font



	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));
	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));
	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));
	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glCullFace(GL_BACK));
	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::GameInit()
{
	//Students should aim to have a better way of managing the scene for coursework

	//loading all resources
	Resources::GetInstance()->AddModel("cube.obj");
	Resources::GetInstance()->AddTexture("wood.jpg");
	Resources::GetInstance()->AddShader(new ShaderProgram(
		ASSET_PATH + "simple.vert",
		ASSET_PATH + "simple.frag"),
		"simple"
	);

	//Font shader
	Resources::GetInstance()->AddShader(new ShaderProgram(
		ASSET_PATH + "font.vert",
		ASSET_PATH + "font.frag"),
		"font"
	);
	//Set projection matrix
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(m_windowWidth), 0.0f, static_cast<GLfloat>(m_windowHeight));
	Resources::GetInstance()->GetShader("font")->Use();
	glUniformMatrix4fv(glGetUniformLocation(Resources::GetInstance()->GetShader("font")->Get(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	

	Entity* a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(
		Resources::GetInstance()->GetModel("cube.obj"),
		Resources::GetInstance()->GetShader("simple"),
		Resources::GetInstance()->GetTexture("wood.jpg")
	));
	a->GetTransform()->SetPosition(glm::vec3(0, -10.f, 0.f));
	RigidBody* rb = new RigidBody();
	a->AddComponent(rb);
	rb->Init(new BoxShape(glm::vec3(100.f, 1.f, 100.f)));
	rb->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));

	for (int i = 0; i < 100; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("cube.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("wood.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, 0.f));
		rb = new RigidBody();
		a->AddComponent(rb);
		rb->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
		a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
	}

	a = new Entity();
	m_entities.push_back(a);
	CameraComp* cc = new CameraComp();
	a->AddComponent(cc);
	cc->Start();
	m_mainCamera->GetParentTransform()->SetPosition(glm::vec3(0, 0, -20));
	m_mainCamera->GetParentTransform()->RotateEulerAxis(180, glm::vec3(0, 1, 0));
}


void Application::Loop()
{
	m_appState = AppState::RUNNING;
	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;
	while (m_appState != AppState::QUITTING)
	{
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:
				//TODO: quit app when user presses escapebreak;
				INPUT->SetKey(event.key.keysym.sym, true);
				break;
			case SDL_KEYUP:
				INPUT->SetKey(event.key.keysym.sym, false);
				break;
				//record when the user releases a key
			case SDL_MOUSEMOTION:
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				break;
			}
		}
		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks -	prevTicks).count() / 100000;

		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;


		//camera control
		if (Input::GetInstance()->GetKey(SDLK_d))
			m_mainCamera->GetParentTransform()->RotateEulerAxis(4 * deltaTime, glm::vec3(0, 1, 0));
		if (Input::GetInstance()->GetKey(SDLK_a))
			m_mainCamera->GetParentTransform()->RotateEulerAxis(-4 * deltaTime, glm::vec3(0, 1, 0));


		//physics update
		Physics::GetInstance()->Update(deltaTime);

		//update and render all entities
		Update(deltaTime);

		//Clear buffer
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Render scene
		Render();
		//Render text
		RenderText(Resources::GetInstance()->GetShader("font"), "Benji is my BroBae <3", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

		SDL_GL_SwapWindow(m_window);
	}
}

void Application::Update(float deltaTime)
{
	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}

void Application::Render()
{
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}
Application::~Application()
{
}
Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}
void Application::Run()
{
	Init();
	Loop();
	Quit();
}
Camera* Application::GetCamera()
{
	return m_mainCamera;
}
void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}

void Application::RenderText(ShaderProgram* shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{   
	// Activate corresponding render state	
	shader->Use();
	glUniform3f(glGetUniformLocation(shader->Get(), "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(fontVAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
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
		x += (ch.Advance >> 6)* scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
