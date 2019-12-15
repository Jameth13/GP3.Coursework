#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <map>
#include <unordered_map>

#include <glew/glew.h>

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>

#include <sdl/SDL.h>
#include <sdl/SDL_opengl.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <bullet/btBulletDynamicsCommon.h>

#include <AL\al.h>
#include <AL\alc.h>

//Fix: error LNK2019: unresolved external symbol _SDL_main referenced in function _main_utf8
#undef main
