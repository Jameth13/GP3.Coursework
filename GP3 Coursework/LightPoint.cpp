#include "LightPoint.h"

LightPoint::LightPoint() {}
LightPoint::~LightPoint() {}

void LightPoint::OnAttach() {}
void LightPoint::OnCollide(RigidBody* other) {}
void LightPoint::Start() {}
void LightPoint::Update() {}

void LightPoint::SetBrightness(float intensity, float dropoff, float range)
{
	this->intensity = intensity;
	this->dropoff = dropoff;
	this->range = range;
}

Vector3 LightPoint::GetBrightness()
{
	return Vector3(intensity, dropoff, range);
}
