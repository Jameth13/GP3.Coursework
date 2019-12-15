#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include "Vector3.h"
#include "Quaternion.h"

class RigidBody;

class Transform : public Component
{
private:
	Vector3 localPos;
	Vector3 localScale;
	Vector3 localEulerAngles;
	Quaternion localRotation;

	Transform* parent = nullptr;

public:
	Transform();
	~Transform();
	Transform(Vector3 pos);
	Transform(Vector3 pos, Vector3 scale, Vector3 rot);
	Transform(Vector3 pos, Vector3 scale, Quaternion quaternion);

	void OnAttach();
	void OnCollide(RigidBody* other);
	void Start();
	void Update();

	glm::mat4 GetModelMatrix();
    glm::mat4 GetTranslationMatrix();

	Vector3 GetPos();
	Vector3 GetScale();
	Vector3 GetEulerAngles();
	Quaternion GetRotation();
	Vector3 GetLocalPos();
	Vector3 GetLocalScale();
	Vector3 GetLocalEulerAngles();
	Quaternion GetLocalRotation();

	void SetPos(Vector3 pos);
	void SetScale(Vector3 scale);
	void SetRotation(Vector3 eulerAngles);
	void SetRotation(Quaternion q);
	void SetLocalPos(Vector3 pos);
	void SetLocalScale(Vector3 scale);
	void SetLocalRotation(Vector3 eulerAngles);
	void SetLocalRotation(Quaternion q);
	
	void TranslateLocal(Vector3 pos);
	void TranslateGlobal(Vector3 pos);
	void RotateLocal(Vector3 eulerAngles);
	void RotateGlobal(Vector3 eulerAngles);

	Vector3 GetForward();
	Vector3 GetUp();
	Vector3 GetRight();

	void SetParent(Transform* parent);
	Transform* GetParent();
};
