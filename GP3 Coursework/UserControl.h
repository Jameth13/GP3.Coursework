#include "Component.h"
#include "RigidBody.h"

class UserControl : public Component
{
public:
	UserControl();
	~UserControl();

	void OnAttach();
	void OnCollide(RigidBody* other);
	void Start();
	void Update();
};
