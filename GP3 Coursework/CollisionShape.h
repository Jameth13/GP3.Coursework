#pragma once
#include "Physics.h"

class CollisionShape
{
private:
	btCollisionShape* m_shape;
public:
	CollisionShape();
	btCollisionShape* Get() { return m_shape; };

	static CollisionShape* InitBox(glm::vec3 halfExtents);
	static CollisionShape* InitSphere(float radius);
	static CollisionShape* InitCapsule(float radius, float height);
	static CollisionShape* InitCylinder(glm::vec3 halfExtents);
	static CollisionShape* InitCone(float radius, float height);
};
