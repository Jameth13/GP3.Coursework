#pragma once

#include "Quaternion.h"
#include "Time.h"
#include "Transform.h"
#include "Vector3.h"

enum class GameState { PLAY, EXIT };

const std::string ASSET_PATH = "..//..//..//assets//";

//Map flags
#define AlbedoMap 0b00000001
#define NormalMap 0b00000010
#define RoughMap  0b00000100
#define MetalMap  0b00001000
#define AOMap     0b00010000

//Shader flags
#define VertShader 0b00000001
#define TescShader 0b00000010
#define TeseShader 0b00000100
#define GeomShader 0b00001000
#define FragShader 0b00010000
#define CompShader 0b00100000
