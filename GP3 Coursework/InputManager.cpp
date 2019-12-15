#include "InputManager.h"

InputManager* InputManager::instance = 0;

InputManager* InputManager::GetInstance()
{
	//Return instance, create an instance if we don't already have one.
	if (instance == 0)
		instance = new InputManager();

	return instance;
}

InputManager::InputManager() {}
InputManager::~InputManager() {}


void InputManager::Update(Display* display, GameState* gameState)
{
	mouseX = 0.0f;
	mouseY = 0.0f;
	keysDown.clear();
	mouseButtonsDown.clear();

	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		if (evnt.type == SDL_QUIT)
		{
			*gameState = GameState::EXIT;
		}

		switch (evnt.type)
		{
		case SDL_MOUSEMOTION:
			if (cursorLocked)
			{
				mouseX = (float)(evnt.motion.x - display->GetWidth() / 2);
				mouseY = (float)(evnt.motion.y - display->GetHeight() / 2);
			}
			else
			{
				mouseX = (float)evnt.motion.xrel;
				mouseY = (float)evnt.motion.yrel;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			AddMouseButton(evnt.button.button);
			if (evnt.button.button == SDL_BUTTON_LEFT)
				LockCursor(true);
			break;

		case SDL_MOUSEBUTTONUP:
			RemoveMouseButton(evnt.button.button);
			break;

		case SDL_KEYDOWN:
			AddKey(evnt.key.keysym.sym);
			if (evnt.key.keysym.sym == SDLK_ESCAPE)
				LockCursor(false);
			break;

		case SDL_KEYUP:
			RemoveKey(evnt.key.keysym.sym);
			break;
		}
	}

	//Potential issue! Different systems react differently to this function.
	if (cursorLocked)
		SDL_WarpMouseInWindow(display->GetWindow(), display->GetWidth() / 2, display->GetHeight() / 2);

}

bool InputManager::GetKey(SDL_Keycode key)
{
	for (unsigned int i = 0u; i < keys.size(); i++)
	{
		if (keys[i] == key)
			return true;
	}
	return false;
}

bool InputManager::GetKeyDown(SDL_Keycode key)
{
	for (unsigned int i = 0u; i < keysDown.size(); i++)
	{
		if (keysDown[i] == key)
			return true;
	}
	return false;
}

bool InputManager::GetKeyToggle(SDL_Keycode key)
{
	for (unsigned int i = 0u; i < keysToggle.size(); i++)
	{
		if (keysToggle[i] == key)
			return true;
	}
	return false;
}

float InputManager::GetMouseX()
{
	return mouseX;
}

float InputManager::GetMouseY()
{
	return mouseY;
}

bool InputManager::GetMouseButton(unsigned int button)
{
	for (unsigned int i = 0u; i < mouseButtons.size(); i++)
	{
		if (mouseButtons[i] == button)
			return true;
	}
	return false;
}

bool InputManager::GetMouseButtonDown(unsigned int button)
{
	for (unsigned int i = 0u; i < mouseButtonsDown.size(); i++)
	{
		if (mouseButtonsDown[i] == button)
			return true;
	}
	return false;
}

void InputManager::Quit()
{
	instance = nullptr;
	delete this;
}


void InputManager::AddKey(SDL_Keycode key)
{
	std::vector<SDL_Keycode>::iterator it;
	for (it = keys.begin(); it != keys.end(); it++)
	{
		if (*it == key)
			return;
	}
	keys.push_back(key);
	keysDown.push_back(key);

	//Update list of toggled keys.
	std::vector<SDL_Keycode>::iterator itDown;
	for (itDown = keysDown.begin(); itDown != keysDown.end(); itDown++)
	{
		std::vector<SDL_Keycode>::iterator itToggle = std::find(keysToggle.begin(), keysToggle.end(), *itDown);
		if (itToggle != keysToggle.end())
			keysToggle.erase(itToggle);
		else
			keysToggle.push_back(*itDown);
	}
}

void InputManager::RemoveKey(SDL_Keycode key)
{
	std::vector<SDL_Keycode>::iterator it;
	for (it = keys.begin(); it != keys.end();)
	{
		if (*it == key)
			it = keys.erase(it);
		else it++;
	}
}


void InputManager::AddMouseButton(unsigned int button)
{
	for (unsigned int i = 0u; i < mouseButtons.size(); i++)
	{
		if (mouseButtons[i] == button)
			return;
	}
	mouseButtons.push_back(button);
	mouseButtonsDown.push_back(button);
}

void InputManager::RemoveMouseButton(unsigned int button)
{
	std::vector<unsigned int>::iterator it;
	for (it = mouseButtons.begin(); it != mouseButtons.end();)
	{
		if (*it == button)
			it = mouseButtons.erase(it);
		else it++;
	}
}


void InputManager::LockCursor(bool lock)
{
	if (lock)
	{
		cursorLocked = true;
		SDL_ShowCursor(SDL_DISABLE);
	}
	else
	{
		cursorLocked = false;
		SDL_ShowCursor(SDL_ENABLE);
	}
}

void InputManager::print()
{
	for (unsigned int i = 0u; i < keys.size(); i++)
	{
		std::cout << keys[i] << ", ";
	}
	std::cout << std::endl;
}
