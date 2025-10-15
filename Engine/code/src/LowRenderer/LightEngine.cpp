#include "LowRenderer/LightEngine.h"

#include "Core/Engine.h"
#include "Core/UI/UI.h"

#include "Resources/Shader.h"
#include "Resources/ResourceManager.h"

ILight* LightEngine::CreateDirectionalLight(const Vector3D& direction, const Vector3D& color)
{
    dirLights.emplace_back(); 
    DirectionalLight& dirLight = dirLights.back();

    dirLight.direction = direction;
    dirLight.color = color; 

    return &dirLight; 
}

ILight* LightEngine::CreatePointLight(const Vector3D& position, const Vector3D& color)
{
    pointLights.emplace_back();
    PointLight& pointLight = pointLights.back();

    pointLight.position = position;
    pointLight.color = color;

    return &pointLight; 
}

ILight* LightEngine::CreateSpotLight(const Vector3D& position, const Vector3D& direction, float spotAngle, float smoothAngle, const Vector3D& color)
{
    spotLights.emplace_back();
    SpotLight& spotLight = spotLights.back();

    spotLight.position = position;
    spotLight.direction = direction;
    spotLight.spotAngle = spotAngle;
    spotLight.spotSmoothAngle = smoothAngle;
    spotLight.color = color;

    return &spotLight;
}

void LightEngine::DeleteLight(ILight* lightToDelete)
{
   for (auto it = dirLights.begin(); it != dirLights.end(); ++it)
   {
        if (&(*it) == lightToDelete)
        {
            dirLights.erase(it);
            return;
        }
    }

    for (auto it = pointLights.begin(); it != pointLights.end(); ++it)
    {
        if (&(*it) == lightToDelete)
        {
            pointLights.erase(it);
            return;
        }
    }

    for (auto it = spotLights.begin(); it != spotLights.end(); ++it)
    {
        if (&(*it) == lightToDelete)
        {
            spotLights.erase(it);
            return;
        }
    }

    std::cerr << "Light not found in any light array!" << std::endl;
}

void LightEngine::ClearLights()
{
    for (auto it = dirLights.begin(); it != dirLights.end(); ++it)
    {
        dirLights.erase(it); 
    }

    for (auto it = pointLights.begin(); it != pointLights.end(); ++it)
    {
        pointLights.erase(it);
    }

    for (auto it = spotLights.begin(); it != spotLights.end(); ++it)
    {
        spotLights.erase(it);
    }
}

void LightEngine::UpdateShaderData() const
{
    GLuint program = Engine::GetInstance()->GetResourceManager()->Get<IShader>("standard_shader.json")->GetProgram();
    glUseProgram(program);

    GLint nbDirLoc = glGetUniformLocation(program, "nbDirectionalLights");
    GLint nbPointLoc = glGetUniformLocation(program, "nbPointLights");
    GLint nbSpotLoc = glGetUniformLocation(program, "nbSpotLights");

    glUniform1i(nbDirLoc, static_cast<GLint>(dirLights.size()));
    glUniform1i(nbPointLoc, static_cast<GLint>(pointLights.size()));
    glUniform1i(nbSpotLoc, static_cast<GLint>(spotLights.size()));

    for (size_t i = 0; i < dirLights.size(); i++) {
        std::string baseName = "dirLights[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(program, (baseName + ".direction").c_str()), 1, &dirLights[i].direction.x);
        glUniform3fv(glGetUniformLocation(program, (baseName + ".color").c_str()), 1, &dirLights[i].color.x);
        glUniform1fv(glGetUniformLocation(program, (baseName + ".intensity").c_str()), 1, &dirLights[i].intensity);
    }

    for (size_t i = 0; i < pointLights.size(); i++) {
        std::string baseName = "pointLights[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(program, (baseName + ".position").c_str()), 1, &pointLights[i].position.x);
        glUniform3fv(glGetUniformLocation(program, (baseName + ".color").c_str()), 1, &pointLights[i].color.x);
        glUniform1fv(glGetUniformLocation(program, (baseName + ".intensity").c_str()), 1, &pointLights[i].intensity);
    }

    for (size_t i = 0; i < spotLights.size(); i++) {
        std::string baseName = "spotLights[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(program, (baseName + ".position").c_str()), 1, &spotLights[i].position.x);
        glUniform3fv(glGetUniformLocation(program, (baseName + ".direction").c_str()), 1, &spotLights[i].direction.x);
        glUniform1f(glGetUniformLocation(program, (baseName + ".spotAngle").c_str()), spotLights[i].spotAngle);
        glUniform1f(glGetUniformLocation(program, (baseName + ".spotSmoothAngle").c_str()), spotLights[i].spotSmoothAngle);
        glUniform3fv(glGetUniformLocation(program, (baseName + ".color").c_str()), 1, &spotLights[i].color.x);
        glUniform1fv(glGetUniformLocation(program, (baseName + ".intensity").c_str()), 1, &spotLights[i].intensity);
    }

    glUseProgram(0);
}
