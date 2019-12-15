#include "DemoGameObject.h"
#include "Oscillate.h"

DemoGameObject::DemoGameObject() {}
DemoGameObject::~DemoGameObject() {}

void DemoGameObject::Awake() {}
void DemoGameObject::Start()
{
	//Transform hierarchy test
	monkeyChildTrans->SetParent(monkeyParent->GetComponent<Transform>());
	monkeyChildTrans->SetLocalPos(Vector3(4.0f, -3.0f, 0.0f));
	//Hard-coded model and texture demo
	missing->GetComponent<Transform>()->SetPos(Vector3(5.0f, 0.0f, 0.0f));
}
bool wait = false;
void DemoGameObject::Update()
{
	//Transform hierarchy test
	monkeyParentTrans->SetPos(Vector3(glm::sin(Time::time * 2) * 3, Time::time / 2, -6.0f));
	monkeyParentTrans->SetRotation(Vector3(Time::time * 2, Time::time * 8, Time::time * 6));

	if (input->GetKeyToggle(SDLK_h))
		monkeyChildTrans->SetParent(nullptr);
	else
		monkeyChildTrans->SetParent(monkeyParent->GetComponent<Transform>());

}

void DemoGameObject::RenderForward() {}
