#pragma once
#include "Utils/Engine.h"

class IModel;
class IMesh;
class IWindow;
class IShader;
class ITexture;
class ILight;
class IMaterial; 
class IRigidBody;

class RenderInterface
{
public :
	ENGINE_API virtual IModel* InstanciateModel() = 0;
	ENGINE_API virtual void DestroyModel(IModel* _model) = 0;

	ENGINE_API virtual IMesh* InstanciateMesh() = 0;
	ENGINE_API virtual void DestroyMesh(IMesh* _mesh) = 0;

	ENGINE_API virtual IWindow* InstanciateWindow() = 0;
	ENGINE_API virtual void DestroyWindow(IWindow* _window) = 0;

	ENGINE_API virtual IShader* InstanciateShader() = 0;
	ENGINE_API virtual void DestroyShader(IShader* _shader) = 0;

	ENGINE_API virtual ITexture* InstanciateTexture() = 0;
	ENGINE_API virtual void DestroyTexture(ITexture* _texture) = 0;

	ENGINE_API virtual ILight* InstanciateLight() = 0;
	ENGINE_API virtual void DestroyLight(ILight* _light) = 0;

	ENGINE_API virtual IMaterial* InstanciateMaterial() = 0;
	ENGINE_API virtual void DestroyMaterial(IMaterial* _material) = 0;
};