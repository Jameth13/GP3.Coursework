#include "UserControl.h"

#include "InputManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "Time.h"


UserControl::UserControl() {}
UserControl::~UserControl() {}

void UserControl::OnAttach() {}
void UserControl::OnCollide(RigidBody* other) {}
void UserControl::Start() {}

void UserControl::Update()
{
	//Define movement speeds.
	float speed = 20.0f * Time::deltaTime;
	float rotationSpeed = 180.0f * Time::deltaTime;

	//Get pointers to input and this game object's Transform.
	InputManager* input = InputManager::GetInstance();
	Transform* transform = gameObject->GetComponent<Transform>();

	//Keyboard movement.
	if (input->GetKey(SDLK_w))
		transform->TranslateLocal(Vector3(0.0f, 0.0f, speed));
	if (input->GetKey(SDLK_s))
		transform->TranslateLocal(Vector3(0.0f, 0.0f, -speed));
	if (input->GetKey(SDLK_a))
		transform->TranslateLocal(Vector3(speed, 0.0f, 0.0f));
	if (input->GetKey(SDLK_d))
		transform->TranslateLocal(Vector3(-speed, 0.0f, 0.0f));

	if (input->GetKey(SDLK_q))
		transform->RotateLocal(Vector3(0.0f, 0.0f, -rotationSpeed));
	if (input->GetKey(SDLK_e))
		transform->RotateLocal(Vector3(0.0f, 0.0f, rotationSpeed));
		
	if (input->GetKey(SDLK_LSHIFT))
		transform->TranslateLocal(Vector3(0.0f, -speed, 0.0f));
	if (input->GetKey(SDLK_SPACE))
		transform->TranslateLocal(Vector3(0.0f, speed, 0.0f));

	//Mouse movement.
	transform->RotateGlobal(Vector3(0.0f, input->GetMouseX() / 16.0f, 0.0f));
	transform->RotateLocal(Vector3(input->GetMouseY() / 16.0f, 0.0f, 0.0f));
}
