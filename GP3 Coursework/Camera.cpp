#include "Camera.h"
#include "CameraComponent.h"
#include "UserControl.h"
#include "Transform.h"


Camera::Camera() {}
Camera::~Camera() {}

void Camera::Awake()
{
	tag = "MainCamera";
	AddComponent<Transform>();
	AddComponent<CameraComponent>();
	AddComponent<UserControl>();
}

void Camera::Start() {}
void Camera::Update() {}
void Camera::RenderForward() {}
