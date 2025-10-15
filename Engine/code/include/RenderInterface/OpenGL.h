#pragma once
#include "RenderInterface/RHI.h"

class RenderInterfaceGL : public RenderInterface
{
public:
	RenderInterfaceGL() = default; 
	~RenderInterfaceGL() = default;

	ENGINE_API IModel* InstanciateModel() override;
	ENGINE_API void DestroyModel(IModel* _model) override;

	ENGINE_API IShader* InstanciateShader() override;
	ENGINE_API void DestroyShader(IShader* _shader) override;

	ENGINE_API IMesh* InstanciateMesh() override;
	ENGINE_API void DestroyMesh(IMesh* _mesh) override;

	ENGINE_API IWindow* InstanciateWindow() override;
	ENGINE_API void DestroyWindow(IWindow* _window) override;

	ENGINE_API ITexture* InstanciateTexture() override;
	ENGINE_API void DestroyTexture(ITexture* _texture) override;

	ENGINE_API ILight* InstanciateLight() override;
	ENGINE_API void DestroyLight(ILight* _light) override;

	ENGINE_API IMaterial* InstanciateMaterial() override;
	ENGINE_API void DestroyMaterial(IMaterial* _material) override;
};