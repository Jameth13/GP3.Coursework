#pragma once
#include <vector>
#include <string>
#include <typeinfo>
#include <stdexcept>
#include "Component.h"

class Shader;
class RigidBody;

class GameObject
{
private:
	std::vector<Component*> components;
	std::vector<Component*> componentsStart;
	void UpdateGameObject();
	void UpdateComponents();
	bool startCalled = false;


public:
	GameObject();
	//Virtual destructor so inherited class' destructor is called.
	virtual ~GameObject();
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void RenderForward() = 0;
	void OnCollide(RigidBody* other);

	template<typename T> void RemoveComponents();
	template<typename T> T* GetComponent();
	template<typename T> std::vector<T*> GetComponents();
	template<typename T, class... Args> T* AddComponent(Args... args);


	//Calls the Start() and Update() of both the game object and its components.
	void UpdateAll();

	std::string tag = "untagged";
	bool toDelete = false;
};


template <typename T>
void GameObject::RemoveComponents()
{
	std::vector<Component*> deleted;
	T* comp = nullptr;

	//Components
	for (auto it = components.begin(); it != components.end(); )
	{
		comp = dynamic_cast<T*>(*it);
		if (comp != NULL)
		{
			deleted.push_back(*it);
			it = components.erase(it);
			delete comp;
		}
		else it++;
	}

	//ComponentsStart
	for (auto itDel = deleted.begin(); itDel != deleted.end(); itDel++)
	{
		for (auto it = componentsStart.begin(); it != componentsStart.end(); )
		{
			if ((*itDel) == (*it))
				it = componentsStart.erase(it);
			else
				it++;
		}
		
	}
}

template <typename T>
T* GameObject::GetComponent()
{
	for (unsigned int i = 0u; i < components.size(); i++)
	{
		T* comp = dynamic_cast<T*>(components[i]);
		if (comp != NULL)
			return comp;
	}
	throw std::invalid_argument("Game object does not have component: " + std::string(typeid(T).name()));
}

template <typename T>
std::vector<T*> GameObject::GetComponents()
{
	std::vector<T*> results;

	for (unsigned int i = 0u; i < components.size(); i++)
	{
		T* comp = dynamic_cast<T*>(components[i]);
		if (comp != NULL)
			results.push_back(comp);
	}
	return results;
}

template <typename T, class... Args>
T* GameObject::AddComponent(Args... args)
{
	Component* c = new T(args...);
	components.push_back(c);
	componentsStart.push_back(c);
	c->gameObject = this;
	c->OnAttach();
	return dynamic_cast<T*>(c);
}
