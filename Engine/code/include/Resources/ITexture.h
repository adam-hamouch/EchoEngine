#pragma once
#include "IResource.h"

#ifdef APIENTRY
#undef APIENTRY
#endif
#include <glad/glad.h>

class TextureGL;

class ITexture : public IResource
{
public:
    ~ITexture() override = default;

    virtual TextureGL* GetGL() = 0;
    virtual GLuint GetTextureID() const = 0;
};
