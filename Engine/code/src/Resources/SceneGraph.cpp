#include "Resources/SceneGraph.h"

#include "Core/DataStructure/GameObject.h"
#include "Core/Engine.h"

#include "LowRenderer/IMesh.h"
#include "LowRenderer/LightEngine.h"
#include "LowRenderer/Text.h"

#include "Resources/ResourceManager.h"
#include "Sound/SoundComponent.h"

#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "GameBehaviour/ScriptsManager.h"
#include "GameBehaviour/GameStateManager.h"

#include "Physics/RigidBody.h"
#include "Physics/BoxCollision.h"
#include "Physics/SphereCollision.h"
#include "Physics/CapsuleCollision.h"

#include <fstream>      
#include <iostream>

#include "LowRenderer/Camera.h"
#include "Utils/FileUtils.h"

SceneGraph::SceneGraph()
{
    world = SetSceneGraph();

    resourceManager = Engine::GetInstance()->GetResourceManager();
    renderInterface = Engine::GetInstance()->GetRenderInterface();

    lightEngine = new LightEngine(); 

    shaders["Standard"] = resourceManager->Get<IShader>("standard_shader.json");
    shaders["Outline"] = resourceManager->Get<IShader>("outline_shader.json");
}

GameObject* SceneGraph::SetSceneGraph()
{
	GameObject* obj = new GameObject();
	return obj;
}

std::string SceneGraph::CheckName(const char* _name)
{
	int i = 0;
	std::string baseName = _name;

	for (auto& obj : objects)
	{
		std::string existingName = obj->name;

		if (existingName == baseName ||
			(existingName.find(baseName + "(") == 0 && existingName[baseName.length()] == '('))
		{
			++i;
		}
	}

	std::string finalName = baseName;
	if (i != 0)
	{
		finalName += "(" + std::to_string(i) + ")";
	}

	return finalName;
}

void SceneGraph::LoadResource(const char* path)
{
    _path = path;

    nlohmann::json jsonData;
    if (!JsonUtils::LoadJsonFile(path, jsonData)) {
        std::cerr << "can't load JSON file" << std::endl;
        return;
    }

    Deserialize(jsonData);
}

void SceneGraph::Serialize()
{
    nlohmann::json jsonData;
    std::unordered_map<std::string, int> nameCounts;

    for (const auto& object : objects)
    {
        std::string baseName = object->name;
        int& count = nameCounts[baseName];
        std::string uniqueName = (count == 0) ? baseName : baseName + "_" + std::to_string(count++);

        nlohmann::json& objJson = jsonData["GameObjects"][uniqueName];

        for (const auto& child : object->children)
        {
            objJson["children"].push_back(child->name);
        }

        objJson["position"] = { JsonUtils::Vector3DToJson(object->transform.pos) };
        objJson["rotation"] = { JsonUtils::Vector3DToJson(object->transform.eulerRot) };
        objJson["scale"] = { JsonUtils::Vector3DToJson(object->transform.scale) };
        objJson["isActive"] = object->isActive;
        objJson["tag"] = object->tag;
        objJson["parent"] = object->parent ? object->parent->name : "null";

        if (IMesh* mesh = object->GetComponent<IMesh>())
        {
            if (IModel* model = mesh->GetModel())
                objJson["meshes"]["model"] = FileUtils::FileName(model->GetPath());

            if (IMaterial* material = mesh->GetMaterial())
                objJson["meshes"]["material"] = FileUtils::FileName(material->GetPath());

            objJson["meshes"]["render"] = mesh->GetRenderMode();
        }
        if (RigidBody* rigidBody = object->GetComponent<RigidBody>())
        {
            nlohmann::json collisionJson;

            objJson["rigidBody"]["motionType"] = rigidBody->GetMotionType();
            objJson["rigidBody"]["collisionCallbacksEnabled"] = rigidBody->HasCollisionCallbacksEnabled();

            CollisionShape* collisionShape = rigidBody->GetCollisionShape();
            if (BoxCollision* box = dynamic_cast<BoxCollision*>(collisionShape))
            {
                collisionJson["type"] = "box";
                collisionJson["size"] = { JsonUtils::Vector3DToJson(box->size) };
            }
            if (SphereCollision* sphere = dynamic_cast<SphereCollision*>(collisionShape))
            {
                collisionJson["type"] = "sphere";
                collisionJson["radius"] = sphere->radius;
            }
            if (CapsuleCollision* capsule = dynamic_cast<CapsuleCollision*>(collisionShape))
            {
                collisionJson["type"] = "capsule";
                collisionJson["radius"] = capsule->radius;
                collisionJson["height"] = capsule->height;
            }

            objJson["rigidBody"]["collision"] = collisionJson;
        }

        if (ILight* light = object->GetComponent<ILight>())
        {
            nlohmann::json lightJson;

            if (DirectionalLight* dir = dynamic_cast<DirectionalLight*>(light))
            {
                lightJson["type"] = "directional";
                lightJson["direction"] = { JsonUtils::Vector3DToJson(dir->direction) };
                lightJson["color"] = { JsonUtils::Vector3DToJson(dir->color) };
            }
            else if (PointLight* pt = dynamic_cast<PointLight*>(light))
            {
                lightJson["type"] = "point";
                lightJson["position"] = { JsonUtils::Vector3DToJson(pt->position) };
                lightJson["color"] = { JsonUtils::Vector3DToJson(pt->color) };
            }
            else if (SpotLight* spot = dynamic_cast<SpotLight*>(light))
            {
                lightJson["type"] = "spot";
                lightJson["position"] = { JsonUtils::Vector3DToJson(spot->position) };
                lightJson["direction"] = { JsonUtils::Vector3DToJson(spot->direction) };
                lightJson["angle"] = spot->spotAngle;
                lightJson["smoothAngle"] = spot->spotSmoothAngle;
                lightJson["color"] = { JsonUtils::Vector3DToJson(spot->color) };
            }

            objJson["light"] = lightJson;
        }

        if (SoundComponent* sound = object->GetComponent<SoundComponent>())
        {
            objJson["sound"] = {
                { "Key", FileUtils::FileName(sound->GetSound()->GetPath()) },
                { "Position", { JsonUtils::Vector3DToJson(sound->GetPosition()) } },
                { "Loop", sound->GetLoop() },
                { "3D", sound->Get3D() },
                { "Volume", sound->GetVolume() }
            };
        }

        if (ScriptComponent* scriptComponent = object->GetComponent<ScriptComponent>())
        {
            objJson["script"] = {
				{ "name", scriptComponent->GetScriptName()}
            }; 
        }

        if (Camera* camera = object->GetComponent<Camera>())
        {
            objJson["camera"] = {
    { "Offset", JsonUtils::Vector3DToJson(camera->offset) }
            };
        }

        if (Text* text = object->GetComponent<Text>())
        {
            objJson["text"] = {
                { "content", text->text },
                { "color", JsonUtils::Vector3DToJson(text->color) },
                { "scale", text->scale },
                { "x", text->x },       
                { "y", text->y }        
            };
        }
    }

    std::ofstream file(_path);
    if (!file) {
        std::cerr << "[Error] Impossible to write in " << _path << std::endl;
        return;
    }

    file << jsonData.dump(4);
    file.close();

    std::cout << "Resources saved in " << _path << std::endl;
}

void SceneGraph::Deserialize(const nlohmann::json& jsonData)
{
    std::unordered_map<std::string, GameObject*> gameObjects;
    std::unordered_map<GameObject*, std::string> pendingParents;
    std::unordered_map<GameObject*, std::vector<std::string>> pendingChildren;

    for (const auto& [gameObjectName, objectData] : jsonData["GameObjects"].items())
    {
        GameObject* obj = new GameObject();
        obj->name = gameObjectName;

        if (objectData.contains("position") && objectData.contains("rotation") && objectData.contains("scale"))
        {
            obj->transform.pos = JsonUtils::ReadVector3D(objectData["position"]);
            obj->transform.eulerRot = JsonUtils::ReadVector3D(objectData["rotation"]);
            obj->transform.scale = JsonUtils::ReadVector3D(objectData["scale"]);
        }

        if (objectData.contains("isActive"))
        {
            obj->isActive = objectData["isActive"].get<bool>();
        }

        if (objectData.contains("tag"))
        {
            obj->tag = objectData["tag"];
        }

        if (objectData.contains("meshes"))
        {
            const auto& meshData = objectData["meshes"];
            IMesh* mesh = renderInterface->InstanciateMesh();
            IMaterial* material = nullptr; 
			IModel* model = nullptr;

            if (meshData.contains("material"))
                material = resourceManager->Get<IMaterial>(meshData["material"].get<std::string>().c_str());
            if (meshData.contains("model"))
                model = resourceManager->Get<IModel>(meshData["model"].get<std::string>().c_str()); 
            if (meshData.contains("render"))
				mesh->SetRenderMode(static_cast<RenderMode>(meshData["render"].get<int>()));

            mesh->Create(
                model,
                material
            );
            obj->components.push_back(mesh);
        }

        if (objectData.contains("rigidBody"))
        {
            const auto& rigidBodyData = objectData["rigidBody"];
            int motionType = rigidBodyData.value("motionType", 0);

            bool collisionCallbacksEnabled = rigidBodyData.value("collisionCallbacksEnabled", false);

            const auto& collisionData = rigidBodyData["collision"];
            std::string type = collisionData.value("type", "");

            CollisionShape* shape = nullptr;

            if (type == "box")
                shape = new BoxCollision(collisionData["size"][0], collisionData["size"][1], collisionData["size"][2]);
            else if (type == "sphere")
                shape = new SphereCollision(collisionData["radius"]);
            else if (type == "capsule")
                shape = new CapsuleCollision(collisionData["radius"], collisionData["height"]);
            else
                std::cerr << "Unknown collision type: " << type << std::endl;

            if (shape)
            {
                RigidBody* rigidBody = new RigidBody(shape, obj, static_cast<JPH::EMotionType>(motionType));
                rigidBody->SetScale(obj->transform.scale);
                rigidBody->SetEnableCollisionCallback(collisionCallbacksEnabled); 

                obj->components.push_back(rigidBody);
                obj->components.push_back(shape);
            }
        }

        if (objectData.contains("light"))
        {
            const auto& lightJson = objectData["light"];

            std::string type = lightJson["type"];

        	ILight* light = nullptr;
            if (type == "spot")
            {
                light = lightEngine->CreateSpotLight(
                    JsonUtils::ReadVector3D(lightJson["position"]),
                    JsonUtils::ReadVector3D(lightJson["direction"]), 
                    lightJson["spotAngle"], 
                    lightJson["smoothAngle"], 
                    JsonUtils::ReadVector3D(lightJson["color"]));
            }
            else if (type == "point")
            {
                light = lightEngine->CreatePointLight(JsonUtils::ReadVector3D(lightJson["position"]), JsonUtils::ReadVector3D(lightJson["color"]));
            }
            else if (type == "directional")
            {
                light = lightEngine->CreateDirectionalLight(JsonUtils::ReadVector3D(lightJson["direction"]), JsonUtils::ReadVector3D(lightJson["color"]));
            }

            if (light)
                obj->components.push_back(light);
        }

        if (objectData.contains("script"))
        {
            ScriptManager* scriptManager = Engine::GetInstance()->GetScriptsManager(); 

            const auto& scriptJson = objectData["script"];

            ScriptComponent* scriptComponent = new ScriptComponent(scriptJson["name"], obj);
        	scriptComponent->Bind(scriptJson["name"]);

            obj->components.push_back(scriptComponent);
        }

        if (objectData.contains("text"))
        {
            const auto& textJson = objectData["text"];

            Text* text = new Text();
            text->text = textJson["content"];
            text->color = JsonUtils::ReadVector3D(textJson["color"]);
            text->scale = textJson["scale"];
            text->x = textJson["x"];
            text->y = textJson["y"];
            text->gameObject = obj;

            obj->components.push_back(text);
        }

        if (objectData.contains("sound"))
        {
            const auto& soundJson = objectData["sound"];

            auto* sound = new SoundComponent();
            if (Sound* asset = resourceManager->Get<Sound>(soundJson["Key"].get<std::string>().c_str()))
            {
                sound->SetSound(asset);
                sound->SetLoop(soundJson.value("Loop", false));
                sound->Set3D(soundJson.value("3D", false));
                sound->SetVolume(soundJson.value("Volume", 1.0f));
                sound->SetPosition(JsonUtils::ReadVector3D(soundJson["Position"]));

                obj->components.push_back(sound);
            }
            else
            {
                delete sound;
            }
        }

        if (objectData.contains("camera"))
        {
            const auto& cameraJson = objectData["camera"];
            auto* camera = new Camera();
            camera->offset = JsonUtils::ReadVector3D(cameraJson["Offset"]);
            camera->SetGameObject(obj);
            obj->components.push_back(camera);
        }

        if (objectData.contains("parent"))
        {
            pendingParents[obj] = objectData["parent"];
        }

        if (objectData.contains("children"))
        {
            for (const auto& childName : objectData["children"])
                pendingChildren[obj].push_back(childName.get<std::string>());
        }

        gameObjects[gameObjectName] = obj;
        objects.push_back(obj);
    }

    for (const auto& [obj, parentName] : pendingParents)
    {
        GameObject* parent = gameObjects[parentName];
        if (parent)
            AddChildren(parent, obj);
        else
            AddChildren(world, obj);
    }

    for (const auto& [obj, childrenNames] : pendingChildren)
    {
        for (const std::string& childName : childrenNames)
        {
            GameObject* child = gameObjects[childName];
            if (child)
            {
                if (child->parent)
                    ChangeParent(obj, child);
                else
                    AddChildren(obj, child);
            }
        }
    }
}

GameObject* SceneGraph::CreateGameObject(const char* _name)
{
	std::string finalName = CheckName(_name);
		
	GameObject* gameObject = new GameObject();
	gameObject->name = _strdup(finalName.c_str());
	objects.push_back(gameObject);
	AddChildren(world, gameObject);

    return gameObject;
}

GameObject* SceneGraph::CreateGameObject(const char* _name, IModel* model, IMaterial* material, Vector3D position, Vector3D rotate, Vector3D scaling)
{
    std::string finalName = CheckName(_name);

    GameObject* gameObject = new GameObject();
    IMesh* mesh = Engine::GetInstance()->GetRenderInterface()->InstanciateMesh();
    gameObject->name = _strdup(finalName.c_str());
    mesh->Create(model, material);

    gameObject->transform.pos = position;
    gameObject->transform.eulerRot = rotate;
    gameObject->transform.scale = scaling;

    gameObject->components.push_back(mesh);
    objects.push_back(gameObject);
    AddChildren(world, gameObject);

    return gameObject;
}

void SceneGraph::Delete(GameObject* object)
{
    GameStateManager* stateManager = Engine::GetInstance()->GetGameStateManager();

    if (!object) return;

    if (object->isInstantiated && stateManager->state == GameState::PLAYING && !stateManager->onLoad)
        stateManager->DeleteRuntimeObject(object);

    for (auto* child : object->children)
    {
        Delete(child); 
    }
    object->children.clear();

	if (object->parent)
    {
        auto& siblings = object->parent->children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), object), siblings.end());
    }

    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());

    delete object;
}

void SceneGraph::UpdateComponent(GameObject* parent) const
{
	for (GameObject* child : parent->children)
	{
        if (child->isActive)
        {
            //RigidBody
            if (RigidBody* rigidBody = child->GetComponent<RigidBody>())
            {
                GameStateManager* gameStateManager = Engine::GetInstance()->GetGameStateManager();

                if (gameStateManager->state == GameState::PLAYING)
                    rigidBody->UpdateTransformFromPhysics();
            }

            //Sound
            if (SoundComponent* sound = child->GetComponent<SoundComponent>())
            {
                sound->SetPosition(child->transform.pos);
            }

            //Collision
            if (BoxCollision* box = child->GetComponent<BoxCollision>())
            {
                box->DrawDebug(child->transform.pos, child->transform.scale);
            }
            if (SphereCollision* sphere = child->GetComponent<SphereCollision>())
            {
                sphere->DrawDebug(child->transform.pos, child->transform.scale);
            }
            if (CapsuleCollision* capsule = child->GetComponent<CapsuleCollision>())
            {
                capsule->DrawDebug(child->transform.pos, child->transform.scale);
            }

            //Text
            if (Text* text = child->GetComponent<Text>())
            {
                text->Draw();
            }
        }
	}
}

void SceneGraph::UpdateScene(Camera* camera)
{
	if (!world) return;

	SetAllTRS(world);
	MultiplyMatrix(world);
	DrawNode(world, camera);
    UpdateComponent(world);
}

void SceneGraph::SetAllTRS(GameObject* parent)
{
	for (GameObject* child : parent->children)
	{
		child->SetLocalMatrix();

		if (child->children.size() > 0)
			SetAllTRS(child);
	}
}

void SceneGraph::MultiplyMatrix(GameObject* parent)
{
	for (auto& child : parent->children)
	{
		Mat4 global = parent->GetGlobalMatrix() * child->GetLocalMatrix();
		child->SetGlobalMatrix(global);
        child->globalTransform.pos = Vector3D(global.Transpose().data[12], global.Transpose().data[13], global.Transpose().data[14]);

		if (!child->children.empty())
			MultiplyMatrix(child);
	}
}

void SceneGraph::DrawNode(GameObject* parent, Camera* camera)
{
	for (auto& child : parent->children)
	{
		IMesh* mesh = child->GetComponent<IMesh>();

		if (mesh && child->isActive)
		{
			mesh->SetModelMatrix(child->GetGlobalMatrix());
			mesh->Draw(camera, shaders);
		}
		if (child->children.size() > 0)
			DrawNode(child, camera);
	}
}

void SceneGraph::AddChildren(GameObject* parent, GameObject* child)
{
	child->parent = parent;
	if (parent)
		parent->children.push_back(child);
}

void SceneGraph::ChangeParent(GameObject* newParent, GameObject* child)
{
	if (child->parent != nullptr)
	{
		auto& siblings = child->parent->children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), child), siblings.end());
	}
	AddChildren(newParent, child);
}