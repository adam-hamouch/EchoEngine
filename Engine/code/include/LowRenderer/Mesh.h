#pragma once
#include <map>

#include "IMesh.h"

class ResourceManager; 
class ShaderGL;
class Camera; 

class MeshGL : public IMesh
{
public:
	MeshGL() = default; 
	~MeshGL() override = default;

	MeshGL* GetGL() override { return this; }

	void Draw(Camera* camera, std::map<std::string, IShader*> shaders) override;

	void Create(IModel* model, IMaterial* material) override;

	void SetModelMatrix(Mat4 matrix) override;
	Mat4 GetModelMatrix() override;

	void SetRenderMode(RenderMode mode) override { renderMode = mode;  }
	RenderMode GetRenderMode() const override { return renderMode; }

	IModel* GetModel() override { return model; }
	IMaterial* GetMaterial() override { return material; }

	void SetModel(IModel* _model) override { model = _model; }
	void SetMaterial(IMaterial* _material) override { material = _material; }

	void DisplayComponentInInspector(InspectorUI* inspector) override;

private:
	IModel* model = nullptr;
	IMaterial* material = nullptr;

	Mat4 mvpMatrix;
	Mat4 modelMatrix;

	RenderMode renderMode = RenderMode::None; 
};
