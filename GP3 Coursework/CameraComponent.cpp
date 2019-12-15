#include "CameraComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include <iostream>


CameraComponent::CameraComponent() {}
CameraComponent::~CameraComponent() {}

void CameraComponent::OnAttach() {}
void CameraComponent::OnCollide(RigidBody* other) {}
void CameraComponent::Start() {}
void CameraComponent::Update() {}

void CameraComponent::InitPersp(float fov, float aspect, float nearClip, float farClip)
{
	this->projection = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
}

void CameraComponent::InitOrtho(float screenWidth, float screenHeight, float nearClip, float farClip)
{
	this->projection = glm::ortho(-screenWidth / 2, screenWidth  / 2, -screenHeight / 2, screenHeight / 2, nearClip, farClip);
}


glm::mat4 CameraComponent::GetView()
{
	Transform* trans = gameObject->GetComponent<Transform>();
	return glm::lookAt<float, glm::precision::highp>(trans->GetPos(), trans->GetPos() + trans->GetForward(), trans->GetUp());
}

glm::mat4 CameraComponent::GetProjection()
{
	return projection;
}
