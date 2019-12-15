#include "Component.h"
#include "Vector3.h"
#include "Transform.h"
#include "RigidBody.h"

class Oscillate : public Component
{
public:
	Oscillate();
	~Oscillate();

	void OnAttach();
	void OnCollide(RigidBody* other);
	void Start();
	void Update();

	void Set(float magnitude, float frequency, Vector3 direction);

private:
	Transform* transform;
	Vector3 originalPos;
	Vector3 change;
	float magnitude = 0.0f;
	float frequency = 0.0f;
	Vector3 direction = Vector3(0.0f, 0.0f, 0.0f);
};
