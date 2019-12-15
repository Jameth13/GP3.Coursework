#pragma once
#include <glm/glm.hpp>
#include "Vector3.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(float w, float x, float y, float z);
	Quaternion(float pitch, float roll, float yaw);
	~Quaternion();

	float w, x, y, z;

	static glm::mat4 toRotationMatrix(Quaternion q);
	static Vector3 toEulerAngle(Quaternion q);
	static Quaternion toQuaternion(float pitch, float roll, float yaw);
	static Quaternion toQuaternion(Vector3 eulerAngles);

	void operator=(Quaternion q);
	bool operator==(Quaternion q);
	Quaternion operator*(Quaternion q);
	Quaternion operator/(Quaternion q);
	Vector3 operator*(Vector3 v);

	Quaternion Conjugate();
	Quaternion Scale(float s);
	float Norm();
	Quaternion Inverse();

	//Used for debugging.
	void print();
};
