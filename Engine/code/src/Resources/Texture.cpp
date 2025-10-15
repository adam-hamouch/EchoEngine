#include "Resources/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

TextureGL::~TextureGL()
{
    glDeleteTextures(1, &textureID);
}

void TextureGL::LoadResource(const char* path)
{
    _path = path;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        GLenum internalFormat = GL_RGB;

        if (nrChannels == 1) {
            format = internalFormat = GL_RED;
        }
        else if (nrChannels == 3) {
            format = GL_RGB;
            //internalFormat = useSRGB ? GL_SRGB : GL_RGB;
        }
        else if (nrChannels == 4) {
            format = GL_RGBA;
            //internalFormat = useSRGB ? GL_SRGB_ALPHA : GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    stbi_image_free(data);
}
