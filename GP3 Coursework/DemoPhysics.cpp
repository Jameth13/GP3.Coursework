#include "pch.h"
#include "DemoPhysics.h"
#include "Physics.h"
#include "RigidBody.h"
#include "CollisionShape.h"
#include "GameObjectManager.h"
#include "ForcePush.h"
#include "Generic.h"

DemoPhysics::DemoPhysics() {}
DemoPhysics::~DemoPhysics(){}

void DemoPhysics::Awake() {}

void DemoPhysics::Start()
{
	GameObjectManager* goMan = GameObjectManager::GetInstance();

	float groundHeight = 2.0f;
	float groundSize= 30.0f;
	float groundThickness = 2.0f;

	Generic* cubeGround = goMan->Instantiate<Generic>("cube.obj", "brickWall", AlbedoMap | NormalMap | RoughMap | AOMap, glm::vec2(2.0f));
	cubeGround->tag = "Ground Cube ";
	cubeGround->AddComponent<RigidBody>();
	cubeGround->GetComponent<Transform>()->SetPos(Vector3(0.0f, groundHeight, -50.0f));
	cubeGround->GetComponent<Transform>()->SetScale(Vector3(groundSize, groundThickness / 2.0f, groundSize));
	cubeGround->GetComponent<RigidBody>()->Init(CollisionShape::InitBox(Vector3(groundSize, groundThickness / 2.0f, groundSize)));
	cubeGround->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());

	float offset = groundHeight + groundThickness;
	for (unsigned int i = 0u; i < 40; i++)
	{
		if (i % 2 == 0)
		{
			Generic* cube = goMan->Instantiate<Generic>("cube.obj", "medieval", AlbedoMap | NormalMap | RoughMap | AOMap);

			std::stringstream ss;
			ss << std::setfill('0') << std::setw(2) << i;
			std::string tag = ss.str();
			cube->tag = "Cube      " + tag;

			cube->AddComponent<RigidBody>();
			cube->AddComponent<ForcePush>();
			cube->GetComponent<Transform>()->SetPos(Vector3(0.0f, offset + 2.0f * i, -50.0f));
			cube->GetComponent<Transform>()->SetScale(Vector3(1.0f, 1.0f, 1.0f));
			cube->GetComponent<RigidBody>()->Init(CollisionShape::InitBox(Vector3(1.0f, 1.0f, 1.0f)));
		}
		else
		{
			Generic* sphere = goMan->Instantiate<Generic>("sphere.obj", "slime", AlbedoMap | NormalMap | RoughMap | MetalMap | AOMap);

			std::stringstream ss;
			ss << std::setfill('0') << std::setw(2) << i;
			std::string tag = ss.str();
			sphere->tag = "Sphere    " + tag;

			sphere->AddComponent<RigidBody>();
			sphere->GetComponent<Transform>()->SetPos(Vector3(0.0f, offset + 2.0f * i, -50.0f));
			sphere->GetComponent<Transform>()->SetScale(Vector3(1.0f, 1.0f, 1.0f));
			sphere->GetComponent<RigidBody>()->Init(CollisionShape::InitSphere(1.0f), 1.0f, glm::vec3(1.0f), 0.6f);
		}
	}
}

void DemoPhysics::Update()
{
	//Render collision text
	Vector3 posScale = Vector3(16.0f, 16.0f, 0.8f);
	Vector3 color = Vector3(0.1f, 0.4f, 0.8f);
	if (input->GetKeyToggle(SDLK_p))
		canvas->RenderText("Number of collisions " + std::to_string(physics->numCollisions), posScale, color);
	else
		canvas->RenderText("Press P to begin physics simulation.", posScale, color);

}
void DemoPhysics::RenderForward() {}
