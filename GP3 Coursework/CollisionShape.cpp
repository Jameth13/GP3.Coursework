#include "pch.h"
#include "CollisionShape.h"

CollisionShape::CollisionShape() {}

CollisionShape* CollisionShape::InitBox(glm::vec3 halfExtents)
{
	CollisionShape* cShape = new CollisionShape();
	cShape->m_shape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
	return cShape;
}

CollisionShape* CollisionShape::InitSphere(float radius)
{
	CollisionShape* cShape = new CollisionShape();
	cShape->m_shape = new btSphereShape(radius);
	return cShape;
}

CollisionShape* CollisionShape::InitCapsule(float radius, float height)
{
	CollisionShape* cShape = new CollisionShape();
	cShape->m_shape = new btCapsuleShape(radius, height);
	return cShape;
}

CollisionShape* CollisionShape::InitCylinder(glm::vec3 halfExtents)
{
	CollisionShape* cShape = new CollisionShape();
	cShape->m_shape = new btCylinderShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
	return cShape;
}

CollisionShape* CollisionShape::InitCone(float radius, float height)
{
	CollisionShape* cShape = new CollisionShape();
	cShape->m_shape = new btConeShape(radius, height);
	return cShape;
}
