#include "pch.h"
#include "common.h"
#include "MainGame.h"
//Components
#include "CameraComponent.h"
#include "UserControl.h"
#include "Mesh.h"
#include "Oscillate.h"
#include "Quaternion.h"

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


	//Initialize and position Cameras
	camera1->GetComponent<CameraComponent>()->InitPersp(_gameDisplay.GetFOV(), _gameDisplay.GetWidth() / _gameDisplay.GetHeight(), 0.01f, 1000.0f);
	camera1->GetComponent<Transform>()->SetPos(Vector3(12.0f, 2.0f, 16.0f));
	camera1->GetComponent<Transform>()->RotateGlobal(Vector3(0.0f, 140.0f, 0.0f));
	
	camera2->GetComponent<CameraComponent>()->InitPersp(_gameDisplay.GetFOV(), _gameDisplay.GetWidth() / _gameDisplay.GetHeight(), 0.01f, 1000.0f);
	camera2->GetComponent<Transform>()->SetPos(Vector3(16.0f, 30.0f, 40.0f));
	camera2->GetComponent<Transform>()->RotateGlobal(Vector3(0.0f, 140.0f, 0.0f));
	camera2->GetComponent<Transform>()->RotateLocal(Vector3(40.0f, 0.0f, 0.0f));
	camera2->tag = "";
	camera2->GetComponent<UserControl>()->enabled = false;


	//Add physics sphere to cameras
	camera1->AddComponent<RigidBody>()->Init(CollisionShape::InitSphere(2.0f));
	camera1->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	camera2->AddComponent<RigidBody>()->Init(CollisionShape::InitSphere(2.0f));
	camera2->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());


	//Terrain
	terrain->GetComponent<Transform>()->SetPos(Vector3(0.0f, -30.0f, 0.0f));
	terrain->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 2.0f));


	//User light
	userLight->GetComponent<Transform>()->SetPos(Vector3(0.0f, 4.0f, 20.0f));
	userLight->GetComponent<Transform>()->SetScale(Vector3(0.3f, 0.3f, 0.2f));
	userLight->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 180.0f, 0.0f));
	userLight->AddComponent<UserControl>()->enabled = false;
	userLight->AddComponent<LightPoint>()->color = userLight->color;
	userLight->GetComponent<LightPoint>()->SetBrightness(20.0f, 24.0f, 100.0f);
	userLight->GetComponent<LightPoint>()->castShadows = true;
}

void MainGame::MainLoop()
{
	while (_gameState != GameState::EXIT)
	{
		//Update delta time.
		Time::Update();

		//Update physics.
		if(input->GetKeyToggle(SDLK_p))
			Physics::GetInstance()->Update();

		//Update managers
		input->Update(&_gameDisplay, &_gameState);
		goMan->Update();

		//Toggle cameras and light control.
		ToggleControl();

		//Render scene
		renderer->Render();

		//Draw Canvas elements
		canvas->Draw();

		//Delete any game objects marked for deletion.
		goMan->CleanUp();

		//Use double buffering.
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

//Swap control between the two cameras and the light source.
void MainGame::ToggleControl()
{
	//Use C to toggle between cameras.
	if (input->GetKeyToggle(SDLK_c))
	{
		camera1->tag = "untagged";
		camera1->GetComponent<UserControl>()->enabled = false;
		camera2->tag = "MainCamera";
		camera2->GetComponent<UserControl>()->enabled = true;
	}
	else
	{
		camera1->tag = "MainCamera";
		camera1->GetComponent<UserControl>()->enabled = true;
		camera2->tag = "untagged";
		camera2->GetComponent<UserControl>()->enabled = false;
	}

	//Control light while right-click is held.
	if (input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		userLight->GetComponent<UserControl>()->enabled = true;
		camera1->GetComponent<UserControl>()->enabled = false;
		camera2->GetComponent<UserControl>()->enabled = false;
	}
	else
		userLight->GetComponent<UserControl>()->enabled = false;
}
