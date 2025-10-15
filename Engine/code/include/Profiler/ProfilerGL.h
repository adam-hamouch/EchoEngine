#pragma once
#include "Profiler/Profiler.h"

class ProfilerGL : public PerformanceProfiler
{
public:
	ProfilerGL() = default; 
	~ProfilerGL() = default;

	ENGINE_API void UpdateFPS() override;
};