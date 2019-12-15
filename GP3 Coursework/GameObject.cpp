#include "GameObject.h"
#include "GameObjectManager.h"
#include "Component.h"
#include "Shader.h"
#include <iostream>
#include <vector>


GameObject::GameObject() {}

GameObject::~GameObject()
{
	for (unsigned int i = 0u; i < components.size(); i++)
		delete components[i];
}

//Calls the Start() and Update() of both the game object and its components.
void GameObject::UpdateAll()
{
	UpdateGameObject();
	UpdateComponents();
}

void GameObject::OnCollide(RigidBody* other)
{
	for (unsigned int i = 0u; i < components.size(); i++)
		components[i]->OnCollide(other);
}

void GameObject::UpdateGameObject()
{
	if (!startCalled)
	{
		startCalled = true;
		Start();
	}
	Update();
}

void GameObject::UpdateComponents()
{
	std::vector<Component*>::iterator it;

	//Start
	for (it = componentsStart.begin(); it != componentsStart.end(); )
	{
		if ((*it)->enabled == true)
		{
			(*it)->Start();
			it = componentsStart.erase(it);
		}
		else
			it++;
	}

	//Update
	for (it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->enabled == true)
			(*it)->Update();
	}
}
