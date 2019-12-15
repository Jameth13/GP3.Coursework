#pragma once
#include "GameObject.h"
#include "InputManager.h"
#include "Canvas.h"
#include "Physics.h"

class DemoPhysics : public GameObject
{
public:
	DemoPhysics();
	~DemoPhysics();

	void Awake();
	void Start();
	void Update();
	void RenderForward();

private:
	GameObjectManager* goMan = GameObjectManager::GetInstance();
	InputManager* input = InputManager::GetInstance();
	Canvas* canvas = Canvas::GetInstance();
	Physics* physics = Physics::GetInstance();
};
