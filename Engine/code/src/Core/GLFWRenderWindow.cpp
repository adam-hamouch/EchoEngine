#include "Core/Window/GLFWRenderWindow.h"
#include "Core/UI/Windows/ContentBrowserUI.h"

void GLFWRenderWindow::CreateWindow(const char* name)
{
    if (!glfwInitialized) 
    {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }
        glfwInitialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    int _width = mode->width;
    int _height = mode->height;

    window = glfwCreateWindow(_width, _height, name, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Erreur : Échec de l'initialisation de GLAD !" << std::endl;
    }



    glfwSetDropCallback(window, [](GLFWwindow*, int count, const char** paths) {
        for (int i = 0; i < count; ++i)
        {
            std::string droppedFile = paths[i];

            ContentBrowserUI::DropFile(droppedFile);
        }
        });
    SetResizeCallback();
}

void GLFWRenderWindow::Destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLFWRenderWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLFWRenderWindow::SetResizeCallback()
{
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
}

void GLFWRenderWindow::Update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool GLFWRenderWindow::WindowShouldClose()
{
    return glfwWindowShouldClose(window);
}

GLFWwindow* GLFWRenderWindow::GetWindow()
{
    return window;
}