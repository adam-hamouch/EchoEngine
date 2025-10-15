#pragma once
#include "Utils/Engine.h"

#include "Camera.h"

class IShader;
class GameStateManager; 

class Skybox {
public:
    ENGINE_API Skybox(IShader* _shader);
    ENGINE_API ~Skybox();

    ENGINE_API void Draw(Camera* camera);
    void SetCubemap(unsigned int textureID);
    unsigned int LoadCubemap();

private:
    unsigned int cubemapTexture;
    unsigned int VAO, VBO;

    IShader* shader;
	GameStateManager* gameStateManager;
};