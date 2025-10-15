#pragma once
#include <vector>

#include "Utils/Engine.h"
#include <optional>
#include <functional>
#include <string>

class UI;
class Camera;
class Inputs;
class IShader;
class ISceneGraph;
class LightSystem;
class PhysicsEngine;
class ResourceManager;
class Transform;
class FontManager;
class PerformanceProfiler;
class ScriptManager;
class GameObject;
class IWindow;

enum class GameState
{
	EDITING,
	PLAYING,
};

class GameStateManager
{
public :
	GameStateManager() = default; 
	~GameStateManager() = default; 

	ENGINE_API void Render(Camera* camera);
	ENGINE_API void ManageStates(Camera* camera, UI* ui);
	ENGINE_API void UpdateScene(Camera* camera, UI* ui, PerformanceProfiler* profiler);
	ENGINE_API void UpdateBehaviour();
	ENGINE_API void DeleteRuntimeObject(GameObject* object);
	ENGINE_API void LoadScene();
	void Play(ScriptManager* scriptManager);
	void Pause();
	void Stop();

	GameState state = GameState::EDITING;

	ISceneGraph* GetCurrentScene() const { return currentGraph;  }
	void SetCurrentScene(ISceneGraph* graph);
	void SubscribeToSceneChange(const std::function<void(ISceneGraph*)>& callback); 
	std::vector<GameObject*> runtimeObject;
	std::optional<std::string> sceneToLoad;
	ISceneGraph* currentGraph = nullptr;
	Camera* cameraGame = nullptr;
	bool shouldOpenCameraPopup = false;
	bool onLoad = false;
private :
	bool isPaused = false;

	std::vector<Transform> transforms;

	std::vector<std::function<void(ISceneGraph*)>> sceneChangeListeners;
};
