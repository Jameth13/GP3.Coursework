#include "pch.h"
#include "Physics.h"
#include "Time.h"
#include "RigidBody.h"
#include"GameObject.h"

Physics* Physics::m_instance = nullptr;

Physics::Physics()
{
	Init();
}

void Physics::Init()
{
	m_world->setGravity(btVector3(0, -9.8, 0));
}

void Physics::AddRigidbody(RigidBody* body, btCollisionShape* collisionShape)
{
	m_rbodies.push_back(body);
	m_world->addRigidBody(body->Get());
	bodyMap[collisionShape] = body;
}

void Physics::PreUpdate()
{
	for (auto r : m_rbodies)
		r->UpdateRigidBody();
}

void Physics::Update()
{
	PreUpdate();
	m_world->stepSimulation(Time::deltaTime, 2);
	for (auto r : m_rbodies)
		r->UpdateParent();
	UpdateCollision();
}

void Physics::UpdateCollision()
{
	numCollisions = m_dispatcher->getNumManifolds();

	for (int i = 0; i < numCollisions; i++)
	{
		btPersistentManifold* contactManifold =	m_dispatcher->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		bodyMap[obA->getCollisionShape()]->gameObject->OnCollide(bodyMap[obB->getCollisionShape()]);
		bodyMap[obB->getCollisionShape()]->gameObject->OnCollide(bodyMap[obA->getCollisionShape()]);
	}
}

void Physics::Quit()
{
	delete m_world;
	delete m_solver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_collisionConfig;

	m_instance = nullptr;
	delete this;
}

btTransform Physics::ConvertTransformToBtTransform(Transform t)
{
	Quaternion rot = t.GetRotation();
	glm::vec3 pos = t.GetPos();
	return btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w), btVector3(pos.x, pos.y, pos.z));
}

Physics* Physics::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Physics();
	}
	return m_instance;
}
