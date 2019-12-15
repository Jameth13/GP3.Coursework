#include "Oscillate.h"

#include "InputManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "Time.h"

Oscillate::Oscillate() {}
Oscillate::~Oscillate() {}

void Oscillate::OnAttach() {}
void Oscillate::OnCollide(RigidBody* other) {}

void Oscillate::Start()
{
	transform = gameObject->GetComponent<Transform>();
	originalPos = transform->GetPos();
}

void Oscillate::Update()
{
	float step = glm::sin(Time::time * frequency) * magnitude;
	transform->SetLocalPos(originalPos + direction * step);
}

void Oscillate::Set(float magnitude, float frequency, Vector3 direction)
{
	this->magnitude = magnitude;
	this->frequency = frequency;
	this->direction = direction.Normalized();
}
