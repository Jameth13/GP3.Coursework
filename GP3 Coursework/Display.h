#pragma once
#include "pch.h"
#include "common.h"

class Display
{
public:
	Display();
	~Display();
	void InitDisplay();
	void SwapBuffer();

	float GetWidth();
	float GetHeight();
	float GetFOV();

	SDL_Window* GetWindow();

	void Quit();

private:

	void returnError(std::string errorString);

	SDL_GLContext glContext; //global variable to hold the context
	SDL_Window* sdlWindow; //holds pointer to out window
	float fov, screenWidth, screenHeight;
};
