#pragma once
#include <vector>

#include "Utils/Engine.h"

class PerformanceProfiler
{
public :
	PerformanceProfiler() = default; 

	ENGINE_API virtual void UpdateFPS() = 0;

	float fps = 0.0f;
	float lastTime = 0.0f;
	float elapsedTime = 0.0f;
	int frames = 0;
	float maxHistory;
	std::vector<float> fpsHistory;
};