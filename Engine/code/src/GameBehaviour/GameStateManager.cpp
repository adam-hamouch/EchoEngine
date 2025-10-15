#include "GameBehaviour/GameStateManager.h"

#include "Core/DataStructure/GameObject.h"
#include "Core/UI/UI.h"
#include "Core/UI/ImGuiEngine.h"
#include "Core/Engine.h"

#include "Physics/PhysicsEngine.h"
#include "Physics/RigidBody.h"
#include "Physics/RenderDebugger.h"

#include "LowRenderer/LightEngine.h"
#include "LowRenderer/Camera.h"

#include "Resources/SceneGraph.h"
#include "Resources/ResourceManager.h"

#include "Profiler/Profiler.h"
#include "GameBehaviour/ScriptsManager.h"

void GameStateManager::Render(Camera* camera)
{
    camera->Update(Engine::GetInstance()->GetImGuiEngine()->DeltaTime());
    currentGraph->GetLightEngine()->UpdateShaderData(); 
    Engine::GetInstance()->GetRenderDebugger()->DrawLines();
    Engine::GetInstance()->GetRenderDebugger()->Clear();
}

void GameStateManager::ManageStates(Camera* camera, UI* ui)
{
    switch (state)
    {
        case GameState::EDITING:
		{
			ui->CreateSceneWindow("Scene Window");
            Render(camera);

            if (shouldOpenCameraPopup)
            {
                if (ImGui::BeginPopupModal("NoCamera", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("No camera found in the scene!\nThe game cannot start without a camera.");
                    ImGui::Separator();

                    if (ImGui::Button("OK"))
                    {
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
            }
            break;
        }
        case GameState::PLAYING:
        {
            ui->CreateSceneWindow("Game");
            cameraGame = nullptr;

            for (auto& obj : currentGraph->GetObjects())
            {
                if (Camera* camGame = obj->GetComponent<Camera>())
                {
                    camGame->UpdateCameraGame();
                    cameraGame = camGame;
                }
            }

            if (!cameraGame)
            {
                ImGui::BeginPopup("NoCamera");
                state = GameState::EDITING;
                shouldOpenCameraPopup = true;
                break;
            }
            else
            {
                shouldOpenCameraPopup = false;
            }

            if (!isPaused)
                UpdateBehaviour();

            break;
        }
    }
}

void GameStateManager::UpdateScene(Camera* camera, UI* ui, PerformanceProfiler* profiler)
{
    if (state == GameState::EDITING)
        currentGraph->UpdateScene(camera);
    else
        currentGraph->UpdateScene(cameraGame);

    ui->DrawFloatingWindow();

    if (state == GameState::EDITING)
    {
        ui->ManageGizmo(camera);
        profiler->UpdateFPS();
    }
}

void GameStateManager::UpdateBehaviour()
{
	Engine::GetInstance()->GetPhysicsEngine()->Update(Engine::GetInstance()->GetImGuiEngine()->DeltaTime());
    Engine::GetInstance()->GetPhysicsEngine()->UpdateCollision(currentGraph->GetObjects());
    ScriptManager* scriptManager = Engine::GetInstance()->GetScriptsManager();
    scriptManager->Update(currentGraph->GetObjects());

    if (sceneToLoad.has_value())
    {
        LoadScene();
        sceneToLoad.reset();
    }
}

void GameStateManager::LoadScene()
{
    ResourceManager* resourcesManager = Engine::GetInstance()->GetResourceManager();
    ScriptManager* scriptManager = Engine::GetInstance()->GetScriptsManager();
    onLoad = true;
    for (auto& obj : runtimeObject)
    {
        currentGraph->Delete(obj);
    }
    runtimeObject.clear();
    onLoad = false;
    ISceneGraph* graph = resourcesManager->Get<SceneGraph>(sceneToLoad.value().c_str());
    SetCurrentScene(graph);
    Play(scriptManager);
}

void GameStateManager::DeleteRuntimeObject(GameObject* object)
{
    auto it = std::find(runtimeObject.begin(), runtimeObject.end(), object);
    if (it != runtimeObject.end())
        runtimeObject.erase(it);
}

void GameStateManager::Play(ScriptManager* scriptManager)
{
    transforms.clear();

    for (GameObject*& obj : currentGraph->GetObjects())
    {
        if (auto* rigidBody = obj->GetComponent<RigidBody>())
        {
            rigidBody->ApplyTransformToPhysics(obj->transform.pos, obj->transform.scale);
            rigidBody->SaveBodyState();
        }

        transforms.push_back(obj->transform);
    }

    isPaused = false;
    scriptManager->Bind(currentGraph->GetObjects());
    scriptManager->Start(currentGraph->GetObjects());
    state = GameState::PLAYING;
}

void GameStateManager::Pause()
{
    isPaused = !isPaused;
}

void GameStateManager::Stop()
{
    state = GameState::EDITING;
    for (auto& obj : runtimeObject)
    {
        currentGraph->Delete(obj);
    }
    runtimeObject.clear();

    for (size_t i = 0; i < currentGraph->GetObjects().size(); ++i)
    {
        currentGraph->GetObjects()[i]->transform = transforms[i];

        if (auto* rigidBody = currentGraph->GetObjects()[i]->GetComponent<RigidBody>())
            rigidBody->RestoreBodyState();
    }

    transforms.clear();
}

void GameStateManager::SetCurrentScene(ISceneGraph* graph)
{
    currentGraph = graph;
    for (auto& callback : sceneChangeListeners)
    {
        if (callback) callback(currentGraph);
    }
}

void GameStateManager::SubscribeToSceneChange(const std::function<void(ISceneGraph*)>& callback)
{
    sceneChangeListeners.push_back(callback);
}
