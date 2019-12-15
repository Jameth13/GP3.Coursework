#include "LightSpot.h"
#include "GameObject.h"

LightSpot::LightSpot() {}
LightSpot::~LightSpot() {}

void LightSpot::OnAttach() {}
void LightSpot::OnCollide(RigidBody* other) {}
void LightSpot::Start() {}
void LightSpot::Update() {}

void LightSpot::SetBrightness(float intensity, float dropoff, float range)
{
	this->intensity = intensity;
	this->dropoff = dropoff;
	this->range = range;
}

Vector3 LightSpot::GetBrightness()
{
	return Vector3(intensity, dropoff, range);
}

void LightSpot::SetAngle(float innerAngle, float outerAngle)
{
	this->innerAngle = innerAngle;
	this->outerAngle = outerAngle;
}

glm::vec2 LightSpot::GetAngle()
{
	return glm::vec2(innerAngle, outerAngle);
}
