#pragma once
#include "pch.h"
#include "common.h"
#include "GameObject.h"

class LightDirectional : public Component
{
public:
	LightDirectional() {};
	~LightDirectional() {};

	void OnAttach() {};
	void OnCollide(RigidBody* other) {};
	void Start() {};
	void Update() {};

	Vector3 color = Vector3(1.0f);
	float intensity = 1.00f;

	bool castShadows = false;
	//X: Width, Y: Height, Z: Depth
	Vector3 shadowMapSizes = Vector3(100.0f);

private:
};
