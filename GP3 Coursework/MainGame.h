#pragma once
#include "pch.h"
#include "common.h"
//Managers
#include "GameObjectManager.h"
#include "InputManager.h"
#include "Renderer.h"
#include "Canvas.h"
#include "AudioManager.h"
//Demos
#include "DemoGameObject.h"
#include "DemoPhysics.h"
#include "DemoLighting.h"
//Physics
#include "Physics.h"


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

	Display _gameDisplay;
	GameState _gameState;

	//Managers
	GameObjectManager* goMan = GameObjectManager::GetInstance();
	InputManager* input = InputManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();
	Canvas* canvas = Canvas::GetInstance();
	Physics* physics = Physics::GetInstance();
	AudioManager* audioMan = AudioManager::GetInstance();

	//Demos
	DemoGameObject* demoGameObject = goMan->Instantiate<DemoGameObject>(&_gameDisplay);
	DemoPhysics* demoPhysics = goMan->Instantiate<DemoPhysics>();
	DemoLighting* demoLighting = goMan->Instantiate<DemoLighting>();
};
