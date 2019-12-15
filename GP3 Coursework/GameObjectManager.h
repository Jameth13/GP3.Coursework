#pragma once
#include "pch.h"
#include "common.h"
#include "GameObject.h"
#include "Camera.h"

class GameObjectManager
{
private:
	//Private constructor, this prevents creating an instance.
	GameObjectManager();

	//Static pointer
	static GameObjectManager* instance;

	std::vector<GameObject*> gameObjects;

public:
	~GameObjectManager();
	//Static method to get pointer
	static GameObjectManager* GetInstance();

	template <typename T, class... Args> T* Instantiate(Args... args);
	template <typename T> T* GetGameObjectTag(std::string tag);
	template <typename T> std::vector<T*> GetGameObjectsTag(std::string tag);
	template <typename T> std::vector<T*> GetComponents();

	void Delete(GameObject* go);
	void Update();

	void RenderForward();

	void CleanUp();
	void DeleteAll();
	void Quit();
};

//Generic variadic function to instantiate game objects with an arbitrary number of parameters.
template <typename T, class... Args>
T* GameObjectManager::Instantiate(Args... args)
{
	//Ensure T inherits from GameObject
	static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");
	T* gameObject = new T(args...);
	gameObjects.push_back(gameObject);
	gameObject->Awake();
	return gameObject;
}

//Find game object by tag.
template <typename T>
T* GameObjectManager::GetGameObjectTag(std::string tag)
{
	for (unsigned int i = 0u; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->tag == tag)
		{
			T* gameObject = dynamic_cast<T*>(gameObjects[i]);
			if (gameObject != NULL)
				return (T*)gameObjects[i];
		}
	}
	throw std::invalid_argument("No game object found with tag: " + tag);
}

//Find game objects by tag.
template <typename T>
std::vector<T*> GameObjectManager::GetGameObjectsTag(std::string tag)
{
	std::vector<T*> results;

	for (unsigned int i = 0u; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->tag == tag)
			results.push_back((T*)gameObjects[i]);
	}
	return results;
}

//Find components by type.
template <typename T>
std::vector<T*> GameObjectManager::GetComponents()
{
	std::vector<T*> results;

	for (unsigned int i = 0u; i < gameObjects.size(); i++)
	{
		std::vector<T*> components = gameObjects[i]->GetComponents<T>();

		for (unsigned int j = 0u; j < components.size(); j++)
		{
			T* comp = dynamic_cast<T*>(components[j]);
			if (comp != NULL)
				results.push_back(comp);
		}
	}
	return results;
}
