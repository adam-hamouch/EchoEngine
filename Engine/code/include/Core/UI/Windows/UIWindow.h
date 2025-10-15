#pragma once

#include <memory>
#include <string>

class Camera;
class PerformanceProfiler;
class GameObject;

class ContentBrowserUI;
class HierarchyUI;
class InspectorUI;
class PerformanceUI;

class UIWindows
{
public:
    UIWindows();
    ~UIWindows() = default;

    void Init();
    void Update(Camera* camera);
    void RenderProfiler(PerformanceProfiler* profiler);

private:
    std::unique_ptr<ContentBrowserUI> contentBrowser;
    std::unique_ptr<HierarchyUI> hierarchy;
    std::unique_ptr<InspectorUI> inspector;
    std::unique_ptr<PerformanceUI> profilerWindow;
};
