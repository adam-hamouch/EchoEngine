#pragma once
#include "IWindow.h"

class GLFWRenderWindow : public IWindow {
public:
    GLFWwindow* window = nullptr;
    bool glfwInitialized = false;

    void CreateWindow(const char* name) override;
    void Destroy() override;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void SetResizeCallback();

    void Update() override;
    bool WindowShouldClose() override;
	GLFWwindow* GetWindow() override;
};