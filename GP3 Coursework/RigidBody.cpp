#include "pch.h"
#include "RigidBody.h"
#include "CollisionShape.h"
#include "GameObject.h"

void RigidBody::OnAttach() {}
void RigidBody::OnCollide(RigidBody* other) {}
void RigidBody::Start() {}
void RigidBody::Update() {}

void RigidBody::Init(CollisionShape* shape, float mass, const glm::vec3	localInertia, float restitution)
{
	btTransform bT = Physics::ConvertTransformToBtTransform(*gameObject->GetComponent<Transform>());
	m_mState = new btDefaultMotionState(bT);
	m_shape = shape;
	btVector3 li = btVector3(localInertia.x, localInertia.y, localInertia.z);
	m_shape->Get()->calculateLocalInertia(mass, li);
	m_rigidBody = new btRigidBody(btScalar(mass), m_mState,	m_shape->Get(), li);
	Physics::GetInstance()->AddRigidbody(this, m_shape->Get());
	m_rigidBody->setSleepingThresholds(0, 0);
	m_rigidBody->setFriction(1);
	m_rigidBody->setRestitution(restitution);
}

void RigidBody::UpdateParent()
{
	btMotionState* motionState = m_rigidBody->getMotionState();
	btTransform btTrans;
	motionState->getWorldTransform(btTrans);

	btVector3 btV3 = btTrans.getOrigin();
	btQuaternion btQ = btTrans.getRotation();

	Transform* trans = gameObject->GetComponent<Transform>();
	trans->SetPos(Vector3(btV3.x(), btV3.y(), btV3.z()));
	trans->SetRotation(Quaternion(btQ.w(), btQ.x(), btQ.y(), btQ.z()));
}

void RigidBody::UpdateRigidBody()
{
	btTransform t =	Physics::ConvertTransformToBtTransform(*gameObject->GetComponent<Transform>());
	m_rigidBody->setWorldTransform(t);
	m_rigidBody->getMotionState()->setWorldTransform(t);
}

void RigidBody::ApplyForce(glm::vec3 force)
{
	m_rigidBody->applyCentralForce(btVector3(force.x, force.y, force.z));
}

void RigidBody::ApplyTorque(glm::vec3 torque)
{
	m_rigidBody->applyTorque(btVector3(torque.x, torque.y, torque.z));
}
