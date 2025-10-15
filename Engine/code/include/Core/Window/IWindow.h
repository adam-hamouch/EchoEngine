#pragma once
#include <iostream>

#ifdef APIENTRY
#undef APIENTRY
#endif
#include "glad/glad.h"

#include "GLFW/glfw3.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

class IWindow
{
public:
    virtual void CreateWindow(const char* name) = 0;
    virtual void Destroy() = 0;
    virtual void Update() = 0;
    virtual bool WindowShouldClose() = 0;
    virtual GLFWwindow* GetWindow() = 0;
};