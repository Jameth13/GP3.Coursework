#pragma once
#include <chrono>

class Time
{
public:
	static float deltaTime;
	static float time;
	static void Update();

private:
	Time();
	static std::chrono::high_resolution_clock timer;
	static std::chrono::steady_clock::time_point start;
	static std::chrono::steady_clock::time_point end;
};
