#include "Quaternion.h"
#include <iostream>
#include <glm/gtc/quaternion.hpp>

//Quaternions tested with: https://quaternions.online/
//Set euler to ZYX order in radians.

Quaternion::Quaternion()
{
	w = 1.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quaternion::Quaternion(float pitch, float roll, float yaw)
{
	Quaternion q = Quaternion::toQuaternion(pitch, roll, yaw);
	*this = q;
}

Quaternion::~Quaternion() {}


//Reference: http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
glm::mat4 Quaternion::toRotationMatrix(Quaternion q)
{
	return glm::mat4(
		1.0f - 2.0f * (q.y * q.y + q.z * q.z),        2.0f * (q.x * q.y + q.w * q.z),        2.0f * (q.x * q.z - q.w * q.y), 0.0f,
		       2.0f * (q.x * q.y - q.w * q.z), 1.0f - 2.0f * (q.x * q.x + q.z * q.z),        2.0f * (q.y * q.z + q.w * q.x), 0.0f,
		       2.0f * (q.x * q.z + q.w * q.y),        2.0f * (q.y * q.z - q.x * q.w), 1.0f - 2.0f * (q.x * q.x + q.y * q.y), 0.0f,
		0.0f,                                  0.0f,                                  0.0f,                                  1.0f
	);
}

//Reference: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Vector3 Quaternion::toEulerAngle(Quaternion q)
{
	float pitch, yaw, roll;

	//Pitch (x-axis rotation)
	float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
	float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	pitch = atan2(sinr_cosp, cosr_cosp);

	//Yaw (y-axis rotation)
	float sinp = 2.0f * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		yaw = copysign(3.14f / 2.0f, sinp);
	else
		yaw = asin(sinp);

	//Yaw (z-axis rotation)
	float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
	float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	roll = atan2(siny_cosp, cosy_cosp);

	return Vector3(pitch, yaw, roll);
}


//Reference: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Quaternion Quaternion::toQuaternion(float pitch, float yaw, float roll)
{
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);
	double cy = cos(-yaw * 0.5);
	double sy = sin(-yaw * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);

	Quaternion q;
	q.w = cr * cy * cp + sr * sy * sp;
	q.x = cr * cy * sp - sr * sy * cp;
	q.y = sr * cy * sp + cr * sy * cp;
	q.z = sr * cy * cp - cr * sy * sp;
	return q;
}

Quaternion Quaternion::toQuaternion(Vector3 eulerAngles)
{
	return Quaternion::toQuaternion(eulerAngles.x, eulerAngles.y, eulerAngles.z);
}

//Operators
void Quaternion::operator=(Quaternion q)
{
	this->w = q.w;
	this->x = q.x;
	this->y = q.y;
	this->z = q.z;
}
bool Quaternion::operator==(Quaternion q)
{
	bool b = true;
	b &= this->w == q.w;
	b &= this->x == q.x;
	b &= this->y == q.y;
	b &= this->z == q.z;
	return b;
}
Quaternion Quaternion::operator*(Quaternion q)
{
	Quaternion result;
	result.w = -x * q.x - y * q.y - z * q.z + w * q.w;
	result.x = x * q.w + y * q.z - z * q.y + w * q.x;
	result.y = -x * q.z + y * q.w + z * q.x + w * q.y;
	result.z = x * q.y - y * q.x + z * q.w + w * q.z;
	return result;
}

Quaternion Quaternion::operator/(Quaternion q)
{
	return ((*this) * (q.Inverse()));

	/* This was inaccurate.
	Quaternion result;
	float divisor = (q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	result.w = (w * q.w + x * q.x + y * q.y + z * q.z) / divisor;
	result.x = (x * q.w - w * q.x - z * q.y + y * q.z) / divisor;
	result.y = (y * q.w + z * q.x - w * q.y - x * q.z) / divisor;
	result.z = (z * q.w - y * q.x + x * q.y - w * q.z) / divisor;
	return result;
	*/
}

Vector3 Quaternion::operator*(Vector3 v)
{
	glm::quat quat = glm::quat(this->w, this->x, this->y, this->z);
	return quat * (glm::vec3)v;
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::Scale(float s)
{
	return Quaternion(w * s, x * s, y * s, z * s);
}
float Quaternion::Norm()
{
	return (w * w + x * x + y * y + z * z);
}
Quaternion Quaternion::Inverse()
{
	return Conjugate().Scale(1.0f / Norm());
}

//Used for debugging.
void Quaternion::print()
{
	std::cout << "W: " << w << " | X: " << x << " | Y: " << y << "  | Z: " << z << std::endl;
}
