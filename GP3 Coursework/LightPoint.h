#pragma once
#include "common.h"
#include "RigidBody.h"

class LightPoint : public Component
{
public:
	LightPoint();
	~LightPoint();

	void OnAttach();
	void OnCollide(RigidBody* other);
	void Start();
	void Update();

	Vector3 color = Vector3(1.0f);
	bool castShadows = false;

	void SetBrightness(float intensity, float dropoff, float range);
	Vector3 GetBrightness();

private:
	float intensity = 1.0f;
	float dropoff = 1.0f;
	float range = 80.0f;
};
