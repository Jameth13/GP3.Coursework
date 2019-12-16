#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	void Awake();
	void Start();
	void Update();
	void RenderForward();
};

