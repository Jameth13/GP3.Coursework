#include "DemoGameObject.h"
#include "Oscillate.h"
#include "CameraComponent.h"
#include "UserControl.h"
#include "CollisionShape.h"
#include "LightPoint.h"

DemoGameObject::DemoGameObject(Display* display)
{
	this->display = display;
}

DemoGameObject::~DemoGameObject() {}

void DemoGameObject::Awake() {}
void DemoGameObject::Start()
{
	//Initialize and position Cameras
	camera1->GetComponent<CameraComponent>()->InitPersp(display->GetFOV(), display->GetWidth() / display->GetHeight(), 0.01f, 1000.0f);
	camera1->GetComponent<Transform>()->SetPos(Vector3(12.0f, 2.0f, 16.0f));
	camera1->GetComponent<Transform>()->RotateGlobal(Vector3(0.0f, 140.0f, 0.0f));

	camera2->GetComponent<CameraComponent>()->InitPersp(display->GetFOV(), display->GetWidth() / display->GetHeight(), 0.01f, 1000.0f);
	camera2->GetComponent<Transform>()->SetPos(Vector3(16.0f, 30.0f, 40.0f));
	camera2->GetComponent<Transform>()->RotateGlobal(Vector3(0.0f, 140.0f, 0.0f));
	camera2->GetComponent<Transform>()->RotateLocal(Vector3(40.0f, 0.0f, 0.0f));
	camera2->tag = "untagged";
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

	//Transform hierarchy test
	monkeyChildTrans->SetParent(monkeyParent->GetComponent<Transform>());
	monkeyChildTrans->SetLocalPos(Vector3(4.0f, -3.0f, 0.0f));

	//Hard-coded model and texture demo
	missing->GetComponent<Transform>()->SetPos(Vector3(5.0f, 0.0f, 0.0f));
}

void DemoGameObject::Update()
{
	//Toggle cameras and light control.
	ToggleControl();

	//Transform hierarchy demo
	monkeyParentTrans->SetPos(Vector3(glm::sin(Time::time * 2) * 3, Time::time / 2, -6.0f));
	monkeyParentTrans->SetRotation(Vector3(Time::time * 2, Time::time * 8, Time::time * 6));

	if (input->GetKeyToggle(SDLK_h))
		monkeyChildTrans->SetParent(nullptr);
	else
		monkeyChildTrans->SetParent(monkeyParent->GetComponent<Transform>());

}

void DemoGameObject::RenderForward() {}

//Swap control between the two cameras and the light source.
void DemoGameObject::ToggleControl()
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

	//Control user light while right-click is held.
	if (input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		userLight->GetComponent<UserControl>()->enabled = true;
		camera1->GetComponent<UserControl>()->enabled = false;
		camera2->GetComponent<UserControl>()->enabled = false;
	}
	else
		userLight->GetComponent<UserControl>()->enabled = false;
}
