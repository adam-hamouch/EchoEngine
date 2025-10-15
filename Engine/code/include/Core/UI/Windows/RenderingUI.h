#pragma once
#ifdef APIENTRY
#undef APIENTRY
#endif
#include "glad/glad.h"

class RenderingUI
{
public:
    RenderingUI() = default; 
    ~RenderingUI() = default;

    void Render(GLuint textureID, int width, int height);
    void ResizeFramebuffer(int width, int height, GLuint& framebuffer, GLuint& texture, GLuint& depthBuffer, GLuint& msaaFrameBuffer, GLuint& msaaTexture, GLuint& msaaDepthBuffer);

private:
    void CreateFramebuffer(int width, int height, GLuint& framebuffer, GLuint& texture, GLuint& depthBuffer, GLuint& msaaFrameBuffer, GLuint& msaaTexture, GLuint& msaaDepthBuffer);
};
