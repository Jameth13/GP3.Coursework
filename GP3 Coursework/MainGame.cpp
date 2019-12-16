#include "pch.h"
#include "common.h"
#include "MainGame.h"

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
}

MainGame::~MainGame() {}

void MainGame::Run()
{
	InitSystems();
	MainLoop();
	Quit();
}

void MainGame::InitSystems()
{
	//Initialize display
	_gameDisplay.InitDisplay();

	//Set clear colour to off-black for easier debugging
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);

	//Audio
	audioMan->LoadSound((ASSET_PATH + "mindspring_materialmusic.wav").c_str(), "BackgroundMusic");
	audioMan->PlaySound("BackgroundMusic");

	//Initialize Renderer
	renderer->Init(_gameDisplay.GetWidth(), _gameDisplay.GetHeight());
	renderer->InitBuffers();
	renderer->InitShaders();
	renderer->InitSkybox("skyboxes//cold");
}

void MainGame::MainLoop()
{
	while (_gameState != GameState::EXIT)
	{
		//Update delta time.
		Time::Update();

		//Update input
		input->Update(&_gameDisplay, &_gameState);

		//Update physics.
		if(input->GetKeyToggle(SDLK_p))
			Physics::GetInstance()->Update();

		//Update gameobjects
		goMan->Update();

		//Render scene
		renderer->Render();

		//Draw UI elements
		canvas->Draw();

		//Delete any game objects marked for deletion.
		goMan->CleanUp();

		//Swap buffers.
		_gameDisplay.SwapBuffer();
	}
}

void MainGame::Quit()
{
	goMan->Quit();
	audioMan->Quit();
	physics->Quit();
	input->Quit();
	renderer->Quit();
	canvas->Quit();

	_gameDisplay.Quit();
}
