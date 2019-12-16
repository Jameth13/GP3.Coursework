#pragma once
#include "pch.h"
#include "common.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "Generic.h"
#include "Display.h"
#include "LightObject.h"

class DemoGameObject : public GameObject
{
public:
	DemoGameObject(Display* display);
	~DemoGameObject();

	void Awake();
	void Start();
	void Update();
	void RenderForward();

private:
	Display* display;
	GameObjectManager* goMan = GameObjectManager::GetInstance();
	InputManager* input = InputManager::GetInstance();

	//Camera
	Camera* camera1 = goMan->Instantiate<Camera>();
	Camera* camera2 = goMan->Instantiate<Camera>();

	//Terrain
	Generic* terrain = goMan->Instantiate<Generic>("terrain.obj", "moss", AlbedoMap | NormalMap | RoughMap | AOMap, glm::vec2(6.0f));

	//User light
	LightObject* userLight = goMan->Instantiate<LightObject>("arrow.obj", "glass.jpg", Vector3(1.5f));

	//Parent-Child hierarchy demo
	Generic* monkeyParent = goMan->Instantiate<Generic>("monkey.obj", Vector3(0.5f, 0.0f, 1.0f));
	Generic* monkeyChild = goMan->Instantiate<Generic>("monkey.obj", "water", AlbedoMap);
	Transform* monkeyParentTrans = monkeyParent->GetComponent<Transform>();
	Transform* monkeyChildTrans = monkeyChild->GetComponent<Transform>();

	//Hard-coded model and texture demo
	Generic* missing = goMan->Instantiate<Generic>("nomodel.obj", "notexture", AlbedoMap);	//Attempt to load a texture and model which do not exist.

	void ToggleControl();
};
