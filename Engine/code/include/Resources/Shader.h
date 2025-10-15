#pragma once
#include "IShader.h"

#include <fstream>
#include <string>
#include <sstream>

class ShaderGL : public IShader
{
public:
    ShaderGL() = default;
    ~ShaderGL() override; 

    void CreateShader(const char* vertexPath, const char* fragmentPath) override;
    GLuint GetProgram() override { return shaderProgram; }

    void LoadResource(const char* path) override;
    std::string GetPath() const override;

    void SetUniformVector2D(std::string name, Vector2D value); 
    void SetUniformVector3D(std::string name, Vector3D value);
    void SetUniformVector4D(std::string name, Vector4D value);
    void SetUniformFloat(std::string name, float value); 
    void SetUniformMatrix4x4(std::string name, Mat4 value); 
    void SetUniformInt(std::string name, int value); 
    void SetUniformTexture(std::string name, GLuint textureID, int slot);
    void SetUniformBool(std::string name, bool value); 

    ShaderGL* GetGL() override { return this; }
private:
    std::string ReadFile(const char* path);
    void CheckCompileErrors(GLuint shader, std::string type);

    GLuint shaderProgram;
    int textureUnit;
};