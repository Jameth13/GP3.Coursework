#include "Time.h"

float Time::deltaTime = 0.0f;
float Time::time = 0.0f;
std::chrono::high_resolution_clock    Time::timer = std::chrono::steady_clock();
std::chrono::steady_clock::time_point Time::start = Time::timer.now();
std::chrono::steady_clock::time_point Time::end   = Time::timer.now();

void Time::Update()
{
	end = timer.now();

	using seconds = std::chrono::duration<float>;
	deltaTime = std::chrono::duration_cast<seconds>(end - start).count();
	if (deltaTime > 1.0f || deltaTime < -1.0f)
		deltaTime = 0.0f;
	time += deltaTime;

	start = timer.now();
}
