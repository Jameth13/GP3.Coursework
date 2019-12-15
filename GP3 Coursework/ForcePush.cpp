#include "ForcePush.h"

#include "InputManager.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "Time.h"


ForcePush::ForcePush() {}
ForcePush::~ForcePush() {}

void ForcePush::OnAttach() {}
void ForcePush::OnCollide(RigidBody* other) {}
void ForcePush::Start() {}

void ForcePush::Update()
{
	//Define physics forces.
	float force = 24.0f;
	float torque = 16.0f;

	//Get pointers to input and this game object's RigidBody.
	InputManager* input = InputManager::GetInstance();
	RigidBody* rb = gameObject->GetComponent<RigidBody>();

	//Keyboard controls.
	if (input->GetKey(SDLK_UP))
		rb->ApplyForce(Vector3(0.0f, force, 0.0f));
	if (input->GetKey(SDLK_DOWN))
		rb->ApplyForce(Vector3(0.0f, -force, 0.0f));
	if (input->GetKey(SDLK_RIGHT))
		rb->ApplyTorque(Vector3(0.0f, -torque, 0.0f));
	if (input->GetKey(SDLK_LEFT))
		rb->ApplyTorque(Vector3(0.0f, torque, 0.0f));
}
