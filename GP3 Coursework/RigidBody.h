#pragma once
#include "Component.h"
#include "Physics.h"

class CollisionShape;

class RigidBody : public Component
{
private:
	CollisionShape* m_shape;
	btRigidBody* m_rigidBody;
	btMotionState* m_mState = nullptr;
	float mass;
public:
	void OnAttach();
	void OnCollide(RigidBody* other);
	void Start();
	void Update();

	void Init(CollisionShape* shape, float mass = 1.f, const glm::vec3 localInertia = glm::vec3(1), float restitution = 0.1f);
	virtual void UpdateParent();
	virtual void UpdateRigidBody();
	btRigidBody* Get() { return m_rigidBody; }
	void ApplyForce(glm::vec3 force);
	void ApplyTorque(glm::vec3 torque);
};
