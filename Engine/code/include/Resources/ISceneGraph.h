#pragma once
#include <Utils/JsonUtils.h> 

#include "Math/Math.h"
#include "Resources/IResource.h"

class IModel;
class ITexture;
class IMaterial;
class Camera;
class GameObject;
class IShader;
class LightEngine; 

class ISceneGraph : public IResource
{
public:
	~ISceneGraph() override = default;

	virtual GameObject* CreateGameObject(const char* name) = 0;
	virtual GameObject* CreateGameObject(const char* name, IModel* model, IMaterial* material, Vector3D position, Vector3D rotate, Vector3D scale) = 0;
	
	virtual void Delete(GameObject* object) = 0;

	virtual void UpdateScene(Camera* camera) = 0;
	virtual void AddChildren(GameObject* parent, GameObject* child) = 0;
	virtual void ChangeParent(GameObject* newParent, GameObject* child) = 0;
	virtual void Serialize() = 0;
	virtual void Deserialize(const nlohmann::json& json) = 0;

	virtual std::vector<GameObject*>& GetObjects() = 0;
	virtual GameObject* GetWorld() = 0;

	virtual LightEngine* GetLightEngine() = 0; 
};
