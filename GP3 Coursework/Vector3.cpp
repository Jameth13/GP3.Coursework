#include "Vector3.h"
#include "Quaternion.h"
#include <glm\gtx\quaternion.hpp>
#include <iostream>


Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float xyz)
{
	this->x = xyz;
	this->y = xyz;
	this->z = xyz;
}
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


Vector3::~Vector3() {}


//Matrices
glm::mat4 Vector3::TransMat()
{
	return glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x,    y,    z,    1.0f
	);
}

glm::mat4 Vector3::ScaleMat()
{
	return glm::mat4(
		x,    0.0f, 0.0f, 0.0f,
		0.0f, y,    0.0f, 0.0f,
		0.0f, 0.0f, z,    0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

glm::mat4 Vector3::RotMat()
{
	return RotZ(z) * RotY(y) * RotX(x);
}


//Normalize
void Vector3::Normalize()
{
	float magnitude = glm::sqrt(x*x + y*y + z*z);
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
}

Vector3 Vector3::Normalized()
{
	float magnitude = glm::sqrt(x*x + y*y + z*z);
	return Vector3(x / magnitude, y / magnitude, z / magnitude);
}


//Conversions
Vector3 Vector3::toRadians()
{
	Vector3 result = Vector3();
	result.x = x * 3.14f / 180.0f;
	result.y = y * 3.14f / 180.0f;
	result.z = z * 3.14f / 180.0f;
	return result;
}

Vector3 Vector3::toDegrees()
{
	Vector3 result = Vector3();
	result.x = x * 180.0f / 3.14f;
	result.y = y * 180.0f / 3.14f;
	result.z = z * 180.0f / 3.14f;
	return result;
}


//Operator overloads
void Vector3::operator=(Vector3 v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}
bool Vector3::operator==(Vector3 v)
{
	bool b = true;
	b &= this->x == v.x;
	b &= this->y == v.y;
	b &= this->z == v.z;
	return b;
}
Vector3 Vector3::operator+(Vector3 v)
{
	Vector3 result;
	result.x = this->x + v.x;
	result.y = this->y + v.y;
	result.z = this->z + v.z;
	return result;
}
Vector3 Vector3::operator-(Vector3 v)
{
	Vector3 result;
	result.x = this->x - v.x;
	result.y = this->y - v.y;
	result.z = this->z - v.z;
	return result;
}
Vector3 Vector3::operator*(Vector3 v)
{
	Vector3 result;
	result.x = this->x * v.x;
	result.y = this->y * v.y;
	result.z = this->z * v.z;
	return result;
}
Vector3 Vector3::operator/(Vector3 v)
{
	Vector3 result;
	result.x = this->x / v.x;
	result.y = this->y / v.y;
	result.z = this->z / v.z;
	return result;
}
void Vector3::operator+=(Vector3 v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}
void Vector3::operator-=(Vector3 v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}
void Vector3::operator*=(Vector3 v2)
{
	this->x *= v2.x;
	this->y *= v2.y;
	this->z *= v2.z;
}
void Vector3::operator/=(Vector3 v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
}

Vector3 Vector3::operator*(float f)
{
	Vector3 result;
	result.x = this->x * f;
	result.y = this->y * f;
	result.z = this->z * f;
	return result;
}



//GLM Type Conversion
//Constructor
Vector3::Vector3(const glm::vec3 &vec3)
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
}
//Assignment
Vector3 & Vector3::operator=(const glm::vec3 &glm)
{
	x = glm.x;
	y = glm.y;
	z = glm.z;
	return *this;
}


//Used for debugging.
void Vector3::print()
{
	std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}


//Matrix rotations
glm::mat4 Vector3::RotX(float x)
{
	return glm::mat4(
		1.0f, 0.0f,         0.0f,        0.0f,
		0.0f,  glm::cos(x), glm::sin(x), 0.0f,
		0.0f, -glm::sin(x), glm::cos(x), 0.0f,
		0.0f, 0.0f,         0.0f,        1.0f
	);
}

glm::mat4 Vector3::RotY(float y)
{
	return glm::mat4(
		glm::cos(y), 0.0f, -glm::sin(y), 0.0f,
		0.0f,        1.0f, 0.0f,        0.0f,
		glm::sin(y), 0.0f, glm::cos(y), 0.0f,
		0.0f,        0.0f, 0.0f,        1.0f
	);
}

glm::mat4 Vector3::RotZ(float z)
{
	return glm::mat4(
		 glm::cos(z), glm::sin(z), 0.0f, 0.0f,
		-glm::sin(z), glm::cos(z), 0.0f, 0.0f,
		 0.0f,        0.0f,        1.0f, 0.0f,
		 0.0f,        0.0f,        0.0f, 1.0f
	);
}
