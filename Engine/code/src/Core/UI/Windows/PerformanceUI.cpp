#include "Core/UI/Windows/PerformanceUI.h"

#include <imgui.h>

#include "Profiler/Profiler.h"

void PerformanceUI::Render(PerformanceProfiler* profiler)
{
	ImGui::Begin("Performance Viewer");

	ImGui::Text("FPS: %.1f", profiler->fps);
	ImGui::PlotLines("FPS History", profiler->fpsHistory.data(), (int)profiler->fpsHistory.size(), 0, nullptr, 0.0f, 200.0f, ImVec2(0, 80));

	ImGui::End(); 
}
