#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "Resources/ResourceManager.h"
#include "Core/UI/UI.h"
#include "GameBehaviour/ScriptsManager.h"
#include "Physics/RigidBody.h"
#include "Physics/BoxCollision.h"
#include "Physics/SphereCollision.h"

GameObject* MonoBehaviour::Instantiate(GameObject* object, Transform transform)
{
    GameStateManager* stateManager = Engine::GetInstance()->GetGameStateManager();
    ISceneGraph* graph = stateManager->GetCurrentScene();
    ResourceManager* resourceManager = Engine::GetInstance()->GetResourceManager();


    if (!graph)
        return nullptr;

    if (!object)
        return nullptr;

    GameObject* clone = graph->CreateGameObject(object->name.c_str(), object->GetComponent<IMesh>()->GetModel(), object->GetComponent<IMesh>()->GetMaterial(), transform.pos, transform.eulerRot, transform.scale);
    clone->GetComponent<IMesh>()->SetRenderMode(object->GetComponent<IMesh>()->GetRenderMode());
    clone->tag = object->tag;
    clone->isInstantiated = true;
    graph->AddChildren(graph->GetWorld(), clone);
    stateManager->runtimeObject.push_back(clone);

    RigidBody* rb = nullptr;

    if (object->GetComponent<ScriptComponent>())
    {
        ScriptComponent* script = new ScriptComponent(object->GetComponent<ScriptComponent>()->GetScriptName(), clone);
        script->Bind(script->GetScriptName());
        clone->components.push_back(script);
        clone->GetComponent<ScriptComponent>()->Start();
    }

    if (object->GetComponent<BoxCollision>())
    {
        BoxCollision* clonedbox = new BoxCollision();
        rb = new RigidBody(clonedbox, clone, object->GetComponent<RigidBody>()->GetMotionType());
        rb->SetEnableCollisionCallback(true);

        clone->components.push_back(clonedbox);
        clone->components.push_back(rb);
    }

    if (object->GetComponent<SphereCollision>())
    {
        SphereCollision* clonedSphere = new SphereCollision();
        rb = new RigidBody(clonedSphere, clone, object->GetComponent<RigidBody>()->GetMotionType());
        rb->SetEnableCollisionCallback(true);

        clone->components.push_back(clonedSphere);
        clone->components.push_back(rb);
    }

    if (rb)
        rb->ApplyTransformToPhysics(transform.pos, transform.scale);

    return clone;
}

GameObject* MonoBehaviour::GetGameObjectByName(const std::string& name)
{
    GameStateManager* stateManager = Engine::GetInstance()->GetGameStateManager();
    ISceneGraph* graph = stateManager->GetCurrentScene();
    if (!graph)
        return nullptr;

    for (int i = 0; i < graph->GetObjects().size(); ++i)
    {
        if (graph->GetObjects()[i]->name == name)
            return graph->GetObjects()[i];
    }
    return nullptr;
}

void MonoBehaviour::Destroy(GameObject* gameObject)
{
    GameStateManager* stateManager = Engine::GetInstance()->GetGameStateManager();
    ISceneGraph* graph = stateManager->GetCurrentScene();
    
    graph->Delete(gameObject);
    Engine::GetInstance()->GetUI()->hierarchy->ClearCurrentObject();
}

void MonoBehaviour::OpenScene(std::string sceneName)
{
    GameStateManager* stateManager = Engine::GetInstance()->GetGameStateManager();
    stateManager->sceneToLoad = sceneName;
}


void MonoBehaviour::OnCollisionEnter(RigidBody* other)
{}

void MonoBehaviour::OnCollisionStay(RigidBody* other)
{}

void MonoBehaviour::OnCollisionExit(RigidBody* other)
{}

ScriptComponent::ScriptComponent(const std::string& name, GameObject* object)
{
    SetScriptName(name);
    SetGameObject(object);
}

void ScriptComponent::Bind(const std::string& scriptName)
{
    instance = createScriptFunc(scriptName.c_str());
    if (instance)
        instance->SetGameObject(GetGameObject());
    else
        std::cerr << "Script '" << scriptName << "' not found in registry.\n";
}

void ScriptComponent::Start()
{
    if (instance) instance->Start();
}

void ScriptComponent::Update()
{
    if (instance) instance->Update();
}

void ScriptComponent::Destroy()
{
    if (instance && destroyScriptFunc)
        destroyScriptFunc(instance);
    instance = nullptr;
}

void ScriptComponent::SetCreateScriptFunc(MonoBehaviour* (*func)(const char*))
{
    createScriptFunc = func;
}

void ScriptComponent::SetDestroyScriptFunc(void(*func)(MonoBehaviour*))
{
    destroyScriptFunc = func;
}

void ScriptComponent::DisplayComponentInInspector(InspectorUI* inspector)
{
    inspector->ScriptDisplay();
}
