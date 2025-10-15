#pragma once

class PerformanceProfiler; 

class PerformanceUI
{
public:
	PerformanceUI() = default; 
	~PerformanceUI() = default; 

	void Render(PerformanceProfiler* profiler);
};