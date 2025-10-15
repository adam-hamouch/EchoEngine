#include "Core/UI/Windows/RenderingUI.h"

#include <imgui.h>
#include <iostream>

void RenderingUI::Render(GLuint textureID, int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2((float)width, (float)height), ImVec2(0, 1), ImVec2(1, 0));
}

void RenderingUI::ResizeFramebuffer(int width, int height, GLuint& framebuffer, GLuint& texture, GLuint& depthBuffer, GLuint& msaaFrameBuffer, GLuint& msaaTexture, GLuint& msaaDepthBuffer)
{
	if (framebuffer)
		glDeleteFramebuffers(1, &framebuffer);
	if (texture)
		glDeleteTextures(1, &texture);
	if (depthBuffer)
		glDeleteRenderbuffers(1, &depthBuffer);

	if (msaaFrameBuffer)
		glDeleteFramebuffers(1, &msaaFrameBuffer);
	if (msaaTexture)
		glDeleteTextures(1, &msaaTexture);
	if (msaaDepthBuffer)
		glDeleteRenderbuffers(1, &msaaDepthBuffer);

	CreateFramebuffer(width, height, framebuffer, texture, depthBuffer, msaaFrameBuffer, msaaTexture, msaaDepthBuffer);
}

void RenderingUI::CreateFramebuffer(int width, int height,
	GLuint& framebuffer, GLuint& texture, GLuint& depthBuffer,
	GLuint& msaaFramebuffer, GLuint& msaaTexture, GLuint& msaaDepthBuffer)
{
	const int MSAA_SAMPLES = 4;

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer (non-MSAA) is not complete!" << std::endl;

	glGenFramebuffers(1, &msaaFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);

	glGenTextures(1, &msaaTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLES, GL_RGB, width, height, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaTexture, 0);

	glGenRenderbuffers(1, &msaaDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, msaaDepthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLES, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msaaDepthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "MSAA Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

