#include "RenderInterface/OpenGL.h"

#include "Core/Window/GLFWRenderWindow.h"
#include "LowRenderer/Mesh.h"
#include "LowRenderer/Light.h"
#include "Resources/Model.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/Material.h"

IModel* RenderInterfaceGL::InstanciateModel()
{
	return new ModelGL();
}

void RenderInterfaceGL::DestroyModel(IModel* _model)
{
	delete _model;
}

IShader* RenderInterfaceGL::InstanciateShader()
{
	return new ShaderGL();
}

void RenderInterfaceGL::DestroyShader(IShader* _shader)
{
	delete _shader;
}

IMesh* RenderInterfaceGL::InstanciateMesh()
{
	return new MeshGL();
}

void RenderInterfaceGL::DestroyMesh(IMesh* _mesh)
{
	delete _mesh;
}

ITexture* RenderInterfaceGL::InstanciateTexture()
{
	return new TextureGL();
}

void RenderInterfaceGL::DestroyTexture(ITexture* _texture)
{
	delete _texture;
}

ILight* RenderInterfaceGL::InstanciateLight()
{
	return new LightGL();
}

void RenderInterfaceGL::DestroyLight(ILight* _light)
{
	delete _light;
}

IMaterial* RenderInterfaceGL::InstanciateMaterial()
{
	return new MaterialGL();
}

void RenderInterfaceGL::DestroyMaterial(IMaterial* _material)
{
	delete _material;
}

IWindow* RenderInterfaceGL::InstanciateWindow()
{
	return new GLFWRenderWindow();
}

void RenderInterfaceGL::DestroyWindow(IWindow* _window)
{
	delete _window;
}