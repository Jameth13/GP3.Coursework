#include "Component.h"
#include "RigidBody.h"

class ForcePush : public Component
{
public:
	ForcePush();
	~ForcePush();

	void OnAttach();
	void OnCollide(RigidBody* other);
	void Start();
	void Update();
};
