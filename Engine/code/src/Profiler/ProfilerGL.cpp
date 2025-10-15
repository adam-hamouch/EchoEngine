#include "Profiler/ProfilerGL.h"
#include "GLFW/glfw3.h"

void ProfilerGL::UpdateFPS() 
{
    float currentTime = (float)glfwGetTime();
    float frameTime = currentTime - lastTime;
    lastTime = currentTime;

    elapsedTime += frameTime;
    frames++;

    if (elapsedTime >= 1.0f)
    {
        fps = (float)frames / elapsedTime;
        frames = 0;
        elapsedTime = 0.0f;

        fpsHistory.push_back(fps);
        if (fpsHistory.size() > maxHistory)
            fpsHistory.erase(fpsHistory.begin());
    }
}