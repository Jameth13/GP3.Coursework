#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>


class Vector3
{
private:
	glm::mat4 RotX(float x);
	glm::mat4 RotY(float y);
	glm::mat4 RotZ(float z);


public:
	Vector3();
	Vector3(float xyz);
	Vector3(float x, float y, float z);
	~Vector3();

	float x, y, z;

	//Matrices
	glm::mat4 TransMat();
	glm::mat4 ScaleMat();
	glm::mat4 RotMat();

	//Normalize
	void Normalize();
	Vector3 Normalized();

	//Conversions
	Vector3 toRadians();
	Vector3 toDegrees();

	//Operators
	void operator=(Vector3 v);
	bool operator==(Vector3 v);
	Vector3 operator+(Vector3 v);
	Vector3 operator-(Vector3 v);
	Vector3 operator*(Vector3 v);
	Vector3 operator/(Vector3 v);
	void operator+=(Vector3 v);
	void operator-=(Vector3 v);
	void operator*=(Vector3 v);
	void operator/=(Vector3 v);

	Vector3 operator*(float f);


	//GLM Type Conversion
	//Constructor
	Vector3(const glm::vec3 &vec3);
	//Assignment
	Vector3& operator= (const glm::vec3& glm);
	//Generic type cast.
	template <typename T, glm::precision P>
	operator glm::tvec3<T, P>() const { return glm::tvec3<T, P>(x, y, z); };


	//Used for debugging
	void print();
};
