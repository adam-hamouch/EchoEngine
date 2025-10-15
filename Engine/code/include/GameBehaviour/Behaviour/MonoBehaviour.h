#pragma once
#include <functional>
#include <iostream>
#include "Core/DataStructure/GameObject.h"
#include "GameBehaviour/ScriptRegistry.h"
#include "Core/Inputs/InputManager.h"
#include "Core/Engine.h"
#include "Resources/SceneGraph.h"
#include "GameBehaviour/GameStateManager.h"
#include "Physics/RigidBody.h"

class ScriptComponent;
class UI;
class ResourceManager;

class MonoBehaviour
{
public:
    virtual ~MonoBehaviour() = default;

    virtual void Start() = 0;
    virtual void Update() = 0;

    ENGINE_API void SetGameObject(GameObject* go) { gameObject = go; }
    ENGINE_API GameObject* GetGameObject() const { return gameObject; }
    ENGINE_API GameObject* GetGameObjectByName(const std::string& name);
    ENGINE_API GameObject* Instantiate(GameObject* object, Transform transform);
    ENGINE_API void Destroy(GameObject* gameObject);
    ENGINE_API void OpenScene(std::string sceneName);

    ENGINE_API virtual void OnCollisionEnter(RigidBody* other);
    ENGINE_API virtual void OnCollisionStay(RigidBody* other);
    ENGINE_API virtual void OnCollisionExit(RigidBody* other);

    template <typename T>
    T* GetComponent()
    {
        return gameObject->GetComponent<T>();
    }

    GameObject* gameObject = nullptr;
    InputManager* inputs = Engine::GetInstance()->GetInputManager();
};

class ScriptComponent : public Component
{
public:

    ScriptComponent(const std::string& name, GameObject* object);

    void Bind(const std::string& scriptName);
    void Start();
    void Update();
    void Destroy();

    template <typename T>
    T* GetScript()
    {
        return dynamic_cast<T*>(instance);
    }

    MonoBehaviour* GetNativeInstance()
    {
        return instance;
    }

    static void SetCreateScriptFunc(MonoBehaviour* (*func)(const char*));
    static void SetDestroyScriptFunc(void (*func)(MonoBehaviour*));

    void DisplayComponentInInspector(InspectorUI* inspector) override;
    void SetScriptName(const std::string& name) { scriptName = name; }
    const std::string& GetScriptName() const { return scriptName; }

private:

    MonoBehaviour* instance = nullptr;
    std::string scriptName;
    static inline MonoBehaviour* (*createScriptFunc)(const char*) = nullptr;
    static inline void (*destroyScriptFunc)(MonoBehaviour*) = nullptr;
};