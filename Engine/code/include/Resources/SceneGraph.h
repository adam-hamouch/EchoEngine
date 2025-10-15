#pragma once
#include "ISceneGraph.h"

class ResourceManager;
class RenderInterface;
class LightEngine; 

class SceneGraph : public ISceneGraph
{
public:
	SceneGraph();
	~SceneGraph() override = default; 

	void LoadResource(const char* path) override;
	void Serialize() override;
	void Deserialize(const nlohmann::json& json) override;

	GameObject* CreateGameObject(const char* name) override;
	GameObject* CreateGameObject(const char* name, IModel* model, IMaterial* material, Vector3D position, Vector3D rotate, Vector3D scale) override;

	void Delete(GameObject* object) override;

	void UpdateScene(Camera* camera) override;
	void AddChildren(GameObject* parent, GameObject* child) override;
	void ChangeParent(GameObject* newParent, GameObject* child) override;

	std::vector<GameObject*>& GetObjects() override { return objects; }
	GameObject* GetWorld() override { return world; }

	LightEngine* GetLightEngine() override { return lightEngine; }

private:
	GameObject* SetSceneGraph();
	void UpdateComponent(GameObject* parent) const;
	void DrawNode(GameObject* parent, Camera* camera);
	void SetAllTRS(GameObject* parent);
	void MultiplyMatrix(GameObject* parent);
	std::string CheckName(const char* name);

	std::vector<GameObject*> objects;
	std::map<std::string, IShader*> shaders;

	GameObject* world = nullptr;

	LightEngine* lightEngine;
	ResourceManager* resourceManager;
	RenderInterface* renderInterface; 
};
