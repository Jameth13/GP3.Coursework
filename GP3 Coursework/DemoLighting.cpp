#include "pch.h"
#include "DemoLighting.h"
#include "Oscillate.h"


DemoLighting::DemoLighting() {}
DemoLighting::~DemoLighting() {}

void DemoLighting::Awake() {}

void DemoLighting::Start()
{
	//Directional light
	lightDirectional->GetComponent<Transform>()->RotateGlobal(Vector3(34.0f, 8.0f, 0.0f));
	lightDirectional->AddComponent<LightDirectional>()->intensity = 0.6f;
	lightDirectional->GetComponent<LightDirectional>()->castShadows = true;
	lightDirectional->GetComponent<LightDirectional>()->shadowMapSizes = Vector3(200.0f, 160.0f, 300.0f);


	//Bloom demmo, light
	pointLightBloom->GetComponent<Transform>()->SetPos(Vector3(8.0f, 10.0f, 1.0f));
	pointLightBloom->AddComponent<Oscillate>()->Set(10.0f, 1.0f, Vector3(1.0f, 0.0f, 0.0f));
	pointLightBloom->AddComponent<LightPoint>()->color = pointLightBloom->color;
	pointLightBloom->GetComponent<LightPoint>()->SetBrightness(1e6f, 3.0f, 6.0f);

	//Bloom demmo, monkeys
	for (int xPos = 0; xPos < 8; xPos++)
		for (int zPos = 0; zPos < 2; zPos++)
		{
			Generic* monkey = goMan->Instantiate<Generic>("monkey.obj", "bricks", AlbedoMap);
			monkey->GetComponent<Transform>()->SetPos(Vector3(xPos * 3.0f, 8.0f, zPos * 3.0f));
		}


	//Red and green light demo
	pointLightRed->GetComponent<Transform>()->SetPos(Vector3(0.0f, 0.0f, 11.0f));
	pointLightRed->AddComponent<Oscillate>()->Set(9.0f, 2.0f, Vector3(0.0f, 0.0f, 2.0f));
	pointLightRed->AddComponent<LightPoint>()->color = pointLightRed->color;
	pointLightRed->GetComponent<LightPoint>()->SetBrightness(6.0f, 10.0f, 80.0f);
	pointLightRed->GetComponent<LightPoint>()->castShadows = true;

	pointLightGreen->GetComponent<Transform>()->SetPos(Vector3(0.0f, 0.0f, 14.0f));
	pointLightGreen->AddComponent<Oscillate>()->Set(12.0f, 3.0f, Vector3(0.0f, 0.0f, 2.0f));
	pointLightGreen->AddComponent<LightPoint>()->color = pointLightGreen->color;
	pointLightGreen->GetComponent<LightPoint>()->SetBrightness(6.0f, 10.0f, 80.0f);
	pointLightGreen->GetComponent<LightPoint>()->castShadows = true;

	monkeyConcrete->GetComponent<Transform>()->SetPos(Vector3(0.0f, 0.0f, 0.0f));


	//Spot light demo
	spotLightMonkeyTrans->SetPos(Vector3(-8.0f, -2.0f, -32.0f));
	spotLightTrans->SetScale(Vector3(0.3f, 0.3f, 0.2f));
	spotLightTrans->SetPos(Vector3(-8.0f, -6.0f, -32.0f));
	spotLight->AddComponent<LightSpot>()->color = Vector3(1.5f, 0.0f, 0.0f);
	spotLight->GetComponent<LightSpot>()->SetBrightness(20.0f, 24.0f, 100.0f);
	spotLight->GetComponent<LightSpot>()->castShadows = true;


	//PBR spheres and lights
	pbrSphere1->GetComponent<Transform>()->SetPos(Vector3(-12.0f, 0.0f, 0.0f));
	pbrSphere1->GetComponent<Transform>()->SetScale(Vector3(3, 3, 3));
	pbrSphere2->GetComponent<Transform>()->SetPos(Vector3(-12.0f, 0.0f, 8.0f));
	pbrSphere2->GetComponent<Transform>()->SetScale(Vector3(3, 3, 3));
	pbrSphere3->GetComponent<Transform>()->SetPos(Vector3(-12.0f, 0.0f, 16.0f));
	pbrSphere3->GetComponent<Transform>()->SetScale(Vector3(3, 3, 3));
	pbrSphere4->GetComponent<Transform>()->SetPos(Vector3(-12.0f, 0.0f, 24.0f));
	pbrSphere4->GetComponent<Transform>()->SetScale(Vector3(3, 3, 3));

	pbrLight1->GetComponent<Transform>()->SetPos(Vector3(-9.0f, 4.0f, 12.0f));
	pbrLight1->AddComponent<Oscillate>()->Set(16.0f, 1.0f, Vector3(0.0f, 0.0f, 1.0f));
	pbrLight1->AddComponent<LightPoint>()->color = Vector3(0.996094f, 0.953125f, 0.894531f);	//Warm white
	pbrLight1->GetComponent<LightPoint>()->SetBrightness(6.0f, 6.0f, 80.0f);
	pbrLight1->GetComponent<LightPoint>()->castShadows = true;

	pbrLight2->GetComponent<Transform>()->SetPos(Vector3(-18.0f, -1.0f, 12.0f));
	pbrLight2->AddComponent<Oscillate>()->Set(16.0f, 1.0f, Vector3(0.0f, 0.0f, -1.0f));
	pbrLight2->AddComponent<LightPoint>()->color = Vector3(0.996094f, 0.953125f, 0.894531f);	//Warm white
	pbrLight2->GetComponent<LightPoint>()->SetBrightness(4.0f, 8.0f, 80.0f);
	pbrLight2->GetComponent<LightPoint>()->castShadows = true;
}

void DemoLighting::Update()
{
	//Directional light controls
	if (!input->GetKeyToggle(SDLK_p))
	{
		if (input->GetKey(SDLK_UP))
			lightDirectional->GetComponent<LightDirectional>()->intensity *= 1.05f;
		if (input->GetKey(SDLK_DOWN))
			lightDirectional->GetComponent<LightDirectional>()->intensity /= 1.05f;
		if (input->GetKey(SDLK_RIGHT))
			lightDirectional->GetComponent<Transform>()->RotateGlobal(Vector3(0.0f, Time::deltaTime * 40, 0.0f));
		if (input->GetKey(SDLK_LEFT))
			lightDirectional->GetComponent<Transform>()->RotateGlobal(Vector3(0.0f, -Time::deltaTime * 40, 0.0f));
	}
	//Red and green light demo
	monkeyConcreteTrans->RotateGlobal(Vector3(0.0f, Time::deltaTime * 3.0f, 0.0f));

	//Spot light demo
	spotLightTrans->RotateGlobal(Vector3(90.0f * Time::deltaTime, 0.0f, 0.0f));
	spotLightMonkeyTrans->RotateGlobal(Vector3(0.0f, 180.0f * Time::deltaTime, 0.0f));
}
void DemoLighting::RenderForward() {}
