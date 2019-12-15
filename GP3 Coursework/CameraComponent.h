#include "pch.h"
#include "Component.h"
#include "RigidBody.h"

class CameraComponent : public Component
{
public:
	CameraComponent();
	~CameraComponent();

	void OnAttach();
	void OnCollide(RigidBody* other);
	void Start();
	void Update();

	void InitPersp(float fov, float aspect, float nearClip, float farClip);
	void InitOrtho(float screenWidth, float screenHeight, float nearClip, float farClip);

	glm::mat4 GetView();
	glm::mat4 GetProjection();

private:
	glm::mat4 projection;
};
