#pragma once
#include "Math/Math.h"
#include <map>

#include "Core/DataStructure/Component.h"

#include "LowRenderer/RenderMode.h"

class IModel;
class IMaterial;
class IShader;
class Camera;
class MeshGL;

class IMesh : public Component
{
public:
	virtual ~IMesh() = default;

	virtual void Create(IModel* model, IMaterial* material) = 0;

	virtual void Draw(Camera* camera, std::map<std::string, IShader*> shaders) = 0;

	virtual void SetModelMatrix(Mat4 matrix) = 0;
	virtual Mat4 GetModelMatrix() = 0;

	virtual void SetRenderMode(RenderMode mode) = 0; 
	virtual RenderMode GetRenderMode() const = 0; 

	virtual IModel* GetModel() = 0;
	virtual IMaterial* GetMaterial() = 0;

	virtual void SetModel(IModel* model) = 0; 
	virtual void SetMaterial(IMaterial* material) = 0; 

	virtual MeshGL* GetGL() = 0;
};