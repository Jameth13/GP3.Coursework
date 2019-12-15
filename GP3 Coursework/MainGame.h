#pragma once
#include "pch.h"
#include "common.h"
//Managers
#include "GameObjectManager.h"
#include "InputManager.h"
#include "Renderer.h"
#include "Canvas.h"
#include "AudioManager.h"
//GameObjects
#include "Generic.h"
#include "LightObject.h"
//Demos
#include "DemoGameObject.h"
#include "DemoPhysics.h"
#include "DemoLighting.h"
//Physics
#include "RigidBody.h"
#include "CollisionShape.h"


class MainGame
{
public:
	MainGame();
	~MainGame();

	void Run();

private:
	void InitSystems();
	void MainLoop();
	void Quit();

	void ToggleControl();

	Display _gameDisplay;
	GameState _gameState;

	//Managers
	GameObjectManager* goMan = GameObjectManager::GetInstance();
	InputManager* input = InputManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();
	Canvas* canvas = Canvas::GetInstance();
	Physics* physics = Physics::GetInstance();
	AudioManager* audioMan = AudioManager::GetInstance();

	//Camera
	Camera* camera1 = goMan->Instantiate<Camera>();
	Camera* camera2 = goMan->Instantiate<Camera>();

	//Terrain
	Generic* terrain = goMan->Instantiate<Generic>("terrain.obj", "moss", AlbedoMap | NormalMap | RoughMap | AOMap, glm::vec2(6.0f));

	//Lights
	LightObject* userLight = goMan->Instantiate<LightObject>("arrow.obj", "glass.jpg", Vector3(1.5f));

	//Demos
	DemoGameObject* demoGameObject = goMan->Instantiate<DemoGameObject>();
	DemoPhysics* demoPhysics = goMan->Instantiate<DemoPhysics>();
	DemoLighting* demoLighting = goMan->Instantiate<DemoLighting>();
};
