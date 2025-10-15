#include "LowRenderer/SkyBox.h"
#include "Resources/Shader.h"

#include "Resources/ResourceManager.h"

#include "stb_image.h"
#include "Core/DataStructure/GameObject.h"
#include "GameBehaviour/GameStateManager.h"
#include "Resources/ISceneGraph.h"

static float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

Skybox::Skybox(IShader* _shader) : shader(_shader)
{
	gameStateManager = Engine::GetInstance()->GetGameStateManager();

    cubemapTexture = LoadCubemap(); 

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete shader;
}

void Skybox::Draw(Camera* camera)
{
    glDepthFunc(GL_LEQUAL);
    ShaderGL* shaderGL = shader->GetGL();
    glUseProgram(shaderGL->GetProgram());

    // Remove translation of view matrix
    Mat4 viewNoTranslation = camera->GetViewMatrix();
    viewNoTranslation.data[3] = 0.0f;
    viewNoTranslation.data[7] = 0.0f;
    viewNoTranslation.data[11] = 0.0f;

    shaderGL->SetUniformMatrix4x4("view", viewNoTranslation);
    shaderGL->SetUniformMatrix4x4("projection", camera->GetProjectionMatrix());
    shaderGL->SetUniformInt("skybox", 0);

    ISceneGraph* sceneGraph = gameStateManager->GetCurrentScene();
	if (sceneGraph)
	{
		for (auto& obj : sceneGraph->GetObjects())
		{
			if (gameStateManager->state == GameState::PLAYING)
			{
                Camera* camera = obj->GetComponent<Camera>();
                if (camera)
                {
                    Mat4 viewNoTranslation = camera->GetViewMatrix();
                    viewNoTranslation.data[3] = 0.0f;
                    viewNoTranslation.data[7] = 0.0f;
                    viewNoTranslation.data[11] = 0.0f;

                    shaderGL->SetUniformMatrix4x4("view", viewNoTranslation);
                    shaderGL->SetUniformMatrix4x4("projection", camera->GetProjectionMatrix());
                }
			}
		}
	}

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

void Skybox::SetCubemap(unsigned int textureID)
{
    glDeleteTextures(1, &cubemapTexture); // Delete previous skybox
    cubemapTexture = textureID;
}

unsigned int Skybox::LoadCubemap()
{
    const std::vector<std::string>& faces = {
        "Assets/Textures/Skybox/right.jpg",  // +X
        "Assets/Textures/Skybox/left.jpg",   // -X
        "Assets/Textures/Skybox/top.jpg",    // +Y
        "Assets/Textures/Skybox/bottom.jpg", // -Y
        "Assets/Textures/Skybox/front.jpg",   // +Z
        "Assets/Textures/Skybox/back.jpg"   // -Z
    };

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
