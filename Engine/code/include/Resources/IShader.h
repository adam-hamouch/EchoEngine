#pragma once
#include "Math/Math.h"
#include "IResource.h"

#ifdef APIENTRY
#undef APIENTRY
#endif
#include <glad/glad.h>

class ShaderGL;

class IShader : public IResource
{
public:
    virtual ~IShader() = default;
    virtual void CreateShader(const char* vertexPath, const char* fragmentPath) = 0;

    virtual GLuint GetProgram() = 0; 

    virtual ShaderGL* GetGL() = 0;
};