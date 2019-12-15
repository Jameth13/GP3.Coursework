#pragma once
#include "common.h"
#include "RigidBody.h"

class LightSpot : public Component
{
public:
	LightSpot();
	~LightSpot();

	void OnAttach();
	void OnCollide(RigidBody* other);
	void Start();
	void Update();

	Vector3 color = Vector3(1.0f);
	bool castShadows = false;

	void SetBrightness(float intensity, float dropoff, float range);
	Vector3 GetBrightness();

	void SetAngle(float innerAngle, float outerAngle);
	glm::vec2 GetAngle();

private:
	float intensity = 1.0f;
	float dropoff = 1.0f;
	float range = 80.0f;

	float innerAngle = 45.0f;
	float outerAngle = 60.0f;
};
