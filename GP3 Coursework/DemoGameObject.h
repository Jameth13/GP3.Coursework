#pragma once
#include "pch.h"
#include "common.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "Generic.h"

class DemoGameObject : public GameObject
{
public:
	DemoGameObject();
	~DemoGameObject();

	void Awake();
	void Start();
	void Update();
	void RenderForward();

private:
	GameObjectManager* goMan = GameObjectManager::GetInstance();
	InputManager* input = InputManager::GetInstance();

	//Hard-coded model and texture demo
	Generic* missing = goMan->Instantiate<Generic>("nomodel.obj", "notexture", AlbedoMap);	//Attempt to load a texture and model which do not exist.

	//Parent-Child hierarchy demo
	Generic* monkeyParent = goMan->Instantiate<Generic>("monkey.obj", Vector3(0.5f, 0.0f, 1.0f));
	Generic* monkeyChild = goMan->Instantiate<Generic>("monkey.obj", "water", AlbedoMap);
	Transform* monkeyParentTrans = monkeyParent->GetComponent<Transform>();
	Transform* monkeyChildTrans = monkeyChild->GetComponent<Transform>();

};
