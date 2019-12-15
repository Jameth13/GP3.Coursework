#include "AssetManager.h"


AssetManager* AssetManager::instance = 0;

AssetManager* AssetManager::GetInstance()
{
	//Return instance, create an instance if we don't already have one.
	if (instance == 0)
		instance = new AssetManager();

	return instance;
}

AssetManager::AssetManager() {}
AssetManager::~AssetManager() {/*To do: How to handle singletons?*/}
