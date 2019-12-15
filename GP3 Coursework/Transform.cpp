#include "pch.h"
#include "Transform.h"
#include <iostream>


Transform::Transform()
{
	this->localPos = Vector3(0.0f);
	this->localScale = Vector3(1.0f);
	this->localEulerAngles = Vector3();
	this->localRotation = Quaternion();
}
Transform::~Transform() {}

Transform::Transform(Vector3 pos)
{
	this->localPos = pos;
	this->localScale = Vector3(1.0f);
	this->localEulerAngles = Vector3();
	this->localRotation = Quaternion();
}

Transform::Transform(Vector3 pos, Vector3 scale, Vector3 eulerAngles)
{
	this->localPos = pos;
	this->localScale = scale;
	Vector3 eulerAnglesRadians = eulerAngles.toRadians();
	this->localEulerAngles = eulerAnglesRadians;
	this->localRotation = Quaternion::toQuaternion(eulerAnglesRadians);
}

Transform::Transform(Vector3 pos, Vector3 scale, Quaternion quaternion)
{
	this->localPos = pos;
	this->localScale = scale;
	this->localRotation = quaternion;
	this->localEulerAngles = Quaternion::toEulerAngle(quaternion);
}


void Transform::OnAttach() {}
void Transform::OnCollide(RigidBody* other) {}
void Transform::Start() {}
void Transform::Update() {}


//Model matrix
glm::mat4 Transform::GetModelMatrix()
{
    glm::mat4 posMat = GetPos().TransMat();
    glm::mat4 scaleMat = GetScale().ScaleMat();
    glm::mat4 rotMat = Quaternion::toRotationMatrix(GetRotation());

    return posMat * rotMat * scaleMat;
}

//Translation matrix
glm::mat4 Transform::GetTranslationMatrix()
{
    return localPos.TransMat();
}


//Getters
Vector3 Transform::GetPos()
{
	if (parent != nullptr)
		return parent->GetPos() + (parent->GetRotation() * localPos);
	else
		return localPos;
}
Vector3 Transform::GetScale()
{
	if (parent != nullptr)
		return parent->GetScale() * localScale;
	else
		return localScale;
}
Vector3 Transform::GetEulerAngles()
{
	if (parent != nullptr)
		return parent->GetEulerAngles() + localEulerAngles.toDegrees();
	else
		return localEulerAngles.toDegrees();
}
Quaternion Transform::GetRotation()
{
	if (parent != nullptr)
		return localRotation * parent->GetRotation();
	else
		return localRotation;
}

//Local getters
Vector3 Transform::GetLocalPos() { return localPos; }
Vector3 Transform::GetLocalScale() { return localScale; }
Vector3 Transform::GetLocalEulerAngles() { return localEulerAngles.toDegrees(); }
Quaternion Transform::GetLocalRotation() { return localRotation; }


//Setters
void Transform::SetPos(Vector3 pos)
{
	if (parent != nullptr)
		this->localPos = parent->GetRotation().Inverse() * (pos - parent->GetPos());
	else
		this->localPos = pos;
}
void Transform::SetScale(Vector3 scale)
{
	if (parent != nullptr)
		this->localScale = scale / parent->GetScale();
	else
		this->localScale = scale;
}

void Transform::SetRotation(Vector3 eulerAngles)
{
	if (parent != nullptr)
	{
		Vector3 radians = eulerAngles.toRadians();
		this->localRotation = Quaternion::toQuaternion(radians) / parent->GetRotation();
		this->localEulerAngles = Quaternion::toEulerAngle(this->localRotation);
	}
	else
	{
		Vector3 radians = eulerAngles.toRadians();
		this->localRotation = Quaternion::toQuaternion(radians);
		this->localEulerAngles = Quaternion::toEulerAngle(this->localRotation);
	}
}
void Transform::SetRotation(Quaternion q)
{
	if (parent != nullptr)
	{
		this->localRotation = q / parent->GetRotation();
		this->localEulerAngles = Quaternion::toEulerAngle(this->localRotation);
	}
	else
	{
		this->localRotation = q;
		this->localEulerAngles = Quaternion::toEulerAngle(this->localRotation);
	}
}

//Local setters
void Transform::SetLocalPos(Vector3 pos) { this->localPos = pos; }
void Transform::SetLocalScale(Vector3 scale) { this->localScale = scale; }

void Transform::SetLocalRotation(Vector3 eulerAngles)
{
	Vector3 radians = eulerAngles.toRadians();
	this->localRotation = Quaternion::toQuaternion(radians);
	this->localEulerAngles = Quaternion::toEulerAngle(this->localRotation);
}
void Transform::SetLocalRotation(Quaternion q)
{
	this->localRotation = q;
	this->localEulerAngles = Quaternion::toEulerAngle(this->localRotation);
}


//Translate
void Transform::TranslateLocal(Vector3 pos)
{
	glm::vec4 v4 = Quaternion::toRotationMatrix(localRotation) * glm::vec4(pos.x, pos.y, pos.z, 0.0f);
	this->localPos += Vector3(v4.x, v4.y, v4.z);
}
void Transform::TranslateGlobal(Vector3 pos)
{
	this->localPos += pos;
}


//Rotate
void Transform::RotateLocal(Vector3 eulerAngles)
{
	Quaternion q = Quaternion::toQuaternion(eulerAngles.toRadians());
	this->localRotation = this->localRotation * q;
	this->localEulerAngles = Quaternion::toEulerAngle(this->localRotation);
}
void Transform::RotateGlobal(Vector3 eulerAngles)
{
	Quaternion q = Quaternion::toQuaternion(eulerAngles.toRadians());
	this->localRotation = q * this->localRotation;
	this->localEulerAngles = Quaternion::toEulerAngle(this->localRotation);
}


//Direction - TODO: Define matrix multiplication so these functions are not reliant on glm.
Vector3 Transform::GetForward()
{
	glm::vec3 v3 = Quaternion::toRotationMatrix(localRotation) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	return Vector3(v3.x, v3.y, v3.z);
}
Vector3 Transform::GetUp()
{
	glm::vec3 v3 = Quaternion::toRotationMatrix(localRotation) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	return Vector3(v3.x, v3.y, v3.z);
}
Vector3 Transform::GetRight()
{
	glm::vec3 v3 = Quaternion::toRotationMatrix(localRotation) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	return Vector3(v3.x, v3.y, v3.z);
}

void Transform::SetParent(Transform* parent)
{
	Vector3 oldPos = GetPos();
	Vector3 oldScale = GetScale();
	Quaternion oldRotation = GetRotation();

	this->parent = parent;

	SetPos(oldPos);
	SetScale(oldScale);
	SetRotation(oldRotation);
}

Transform* Transform::GetParent()
{
	return parent;
}
