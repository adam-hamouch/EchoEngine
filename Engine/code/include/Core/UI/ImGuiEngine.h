#pragma once
#include "Utils/Engine.h"

class IWindow;

class ENGINE_API ImGuiEngine
{
public:
    void Init(IWindow* window); 
    void BeginFrame(); 
    void EndFrame(); 
    void Destroy();
    float DeltaTime(); 
    bool CreateButton(const char* name);
    void SetupImGuiStyle();
    void Render();
    void End();
};
