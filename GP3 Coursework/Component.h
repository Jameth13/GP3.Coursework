#pragma once
#include "pch.h"

//Resolve circular dependency with forward declaration.
class GameObject;
class RigidBody;

class Component
{
public:
	Component() {};
	//Virtual destructor so inherited class' destructor is called.
	virtual ~Component() {};

	GameObject* gameObject = nullptr;

	virtual void OnAttach() = 0;
	virtual void OnCollide(RigidBody* other) = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;

	bool enabled = true;
};
