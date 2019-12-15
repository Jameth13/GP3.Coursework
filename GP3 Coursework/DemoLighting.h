#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Shader.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "Generic.h"
#include "LightObject.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"

class DemoLighting : public GameObject
{
public:
	DemoLighting();
	~DemoLighting();

	void Awake();
	void Start();
	void Update();
	void RenderForward();

private:
	GameObjectManager* goMan = GameObjectManager::GetInstance();
	InputManager* input = InputManager::GetInstance();

	//Directional light
	Generic* lightDirectional = goMan->Instantiate<Generic>();

	//Bloom demo
	LightObject* pointLightBloom = goMan->Instantiate<LightObject>("sphere.obj", "glass.jpg", Vector3(1.5f));

	//Red and green light demo
	LightObject* pointLightRed = goMan->Instantiate<LightObject>("sphere.obj", "glass.jpg", Vector3(1.5f, 0.0f, 0.0f));
	LightObject* pointLightGreen = goMan->Instantiate<LightObject>("sphere.obj", "glass.jpg", Vector3(0.0f, 1.5f, 0.0f));
	Generic* monkeyConcrete = goMan->Instantiate<Generic>("monkey.obj", "concrete", AlbedoMap | NormalMap | RoughMap | AOMap);
	Transform* monkeyConcreteTrans = monkeyConcrete->GetComponent<Transform>();

	//Spot light demo
	LightObject* spotLight = goMan->Instantiate<LightObject>("torch.obj", "glass.jpg", Vector3(1.5f, 0.0f, 0.0f));
	Generic* spotLightMonkey = goMan->Instantiate<Generic>("monkey.obj", "water", AlbedoMap);
	Transform* spotLightTrans = spotLight->GetComponent<Transform>();
	Transform* spotLightMonkeyTrans = spotLightMonkey->GetComponent<Transform>();

	//Spheres and lights
	Generic* pbrSphere1 = goMan->Instantiate<Generic>("sphere.obj", "rust", AlbedoMap | NormalMap | RoughMap | AOMap);
	Generic* pbrSphere2 = goMan->Instantiate<Generic>("sphere.obj", "brickWall", AlbedoMap | NormalMap | RoughMap | AOMap);
	Generic* pbrSphere3 = goMan->Instantiate<Generic>("sphere.obj", "slate", AlbedoMap | NormalMap | RoughMap | AOMap);
	Generic* pbrSphere4 = goMan->Instantiate<Generic>("sphere.obj", "tile", AlbedoMap | NormalMap | RoughMap | AOMap);

	LightObject* pbrLight1 = goMan->Instantiate<LightObject>("sphere.obj", "glass.jpg", Vector3(0.996094f, 0.953125f, 0.894531f) * 1.5f);
	LightObject* pbrLight2 = goMan->Instantiate<LightObject>("sphere.obj", "glass.jpg", Vector3(0.996094f, 0.953125f, 0.894531f) * 1.5f);

};
