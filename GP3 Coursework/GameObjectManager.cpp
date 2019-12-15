#include "GameObjectManager.h"
#include "MeshRenderer.h"

GameObjectManager* GameObjectManager::instance = nullptr;

GameObjectManager::GameObjectManager() {}
GameObjectManager::~GameObjectManager() {}

GameObjectManager* GameObjectManager::GetInstance()
{
	//Return instance, create an instance if we don't already have one.
	if (instance == 0)
		instance = new GameObjectManager();

	return instance;
}

void GameObjectManager::Delete(GameObject* go)
{
	go->toDelete = true;
}

void GameObjectManager::Update()
{
	for (unsigned int i = 0u; i < gameObjects.size(); i++)
	{
		if (!gameObjects[i]->toDelete)
			gameObjects[i]->UpdateAll();
	}
}


void GameObjectManager::RenderForward()
{
	for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		//Update all game objects that are not scheduled to be deleted
		if (!(*it)->toDelete)
			(*it)->RenderForward();
	}
}


void GameObjectManager::CleanUp()
{
	for (auto it = gameObjects.begin(); it != gameObjects.end(); )
	{
		if ((*it)->toDelete)
		{
			delete *it;
			it = gameObjects.erase(it);
		}
		else it++;
	}
}

void GameObjectManager::DeleteAll()
{
	for (auto it = gameObjects.begin(); it != gameObjects.end(); )
	{
		delete* it;
		it = gameObjects.erase(it);
	}
}

void GameObjectManager::Quit()
{
	DeleteAll();

	instance = nullptr;
	delete this;
}
