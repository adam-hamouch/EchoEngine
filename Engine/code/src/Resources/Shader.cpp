#include "Resources/Shader.h"

#include "Utils/JsonUtils.h"

ShaderGL::~ShaderGL()
{
    glDeleteProgram(shaderProgram);
}

void ShaderGL::LoadResource(const char* path)
{
    _path = path; 

    nlohmann::json jsonData;
    if (JsonUtils::LoadJsonFile(path, jsonData))
    {
        if (jsonData.contains("vertex") && jsonData.contains("fragment"))
        {
            std::string vertexPath = jsonData["vertex"];
            std::string fragmentPath = jsonData["fragment"];
            CreateShader(vertexPath.c_str(), fragmentPath.c_str());
        }
        else
        {
            std::cerr << "Shader JSON missing 'vertex' or 'fragment' keys: " << path << std::endl;
        }
    }
}

std::string ShaderGL::GetPath() const
{
    return _path;
}

void ShaderGL::CreateShader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    CheckCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

std::string ShaderGL::ReadFile(const char* path)
{
    std::ifstream file;
    std::stringstream buffer;
    file.open(path);
    if (file.is_open()) {
        buffer << file.rdbuf();
        file.close();
    }
    else {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    return buffer.str();
}

void ShaderGL::CheckCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
}

void ShaderGL::SetUniformVector2D(std::string name, Vector2D value)
{
    int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void ShaderGL::SetUniformVector3D(std::string name, Vector3D value)
{
    int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void ShaderGL::SetUniformVector4D(std::string name, Vector4D value)
{
    int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void ShaderGL::SetUniformFloat(std::string name, float value)
{
    int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1f(location, value);
}

void ShaderGL::SetUniformMatrix4x4(std::string name, Mat4 value)
{
    int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_TRUE, value.data);
}

void ShaderGL::SetUniformInt(std::string name, int value)
{
    int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1i(location, value);
}

void ShaderGL::SetUniformTexture(std::string name, GLuint textureID, int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLint location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1i(location, slot);
}

void ShaderGL::SetUniformBool(std::string name, bool value)
{
    GLint location = glGetUniformLocation(shaderProgram, name.c_str());
    if (location != -1)
        glUniform1i(location, static_cast<GLint>(value));
}

