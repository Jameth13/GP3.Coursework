#include "Display.h"


Display::Display()
{
	sdlWindow = nullptr; //initialise to generate null access violation for debugging. 
	fov = 60;
	screenWidth = 1280;
	screenHeight = 720; 
}

Display::~Display() {}

void Display::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void Display::SwapBuffer()
{
	SDL_GL_SwapWindow(sdlWindow); //swap buffers
}

float Display::GetWidth()
{
	return screenWidth;
}
float Display::GetHeight()
{
	return screenHeight;
}
float Display::GetFOV()
{
	return fov;
}

SDL_Window* Display::GetWindow()
{
	return sdlWindow;
}

void Display::Quit()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(sdlWindow);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}

void Display::InitDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   

	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	sdlWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL); // create window
	
	if (sdlWindow == nullptr)
	{
		returnError("window failed to create");
	}


	glContext = SDL_GL_CreateContext(sdlWindow);
	
	if (glContext == nullptr)
	{
		returnError("SDL_GL context failed to create");
	}


	GLenum error = glewInit();

	if (error != GLEW_OK)
	{
		returnError("GLEW failed to initialise");
	}
}
