#pragma once
#include "Utils/Engine.h"

class RenderInterface; 
class RenderDebugger;
class PhysicsEngine; 
class AudioEngine;
class ResourceManager;
class InputManager;
class ImGuiEngine;
class RenderTextManager;
class GameStateManager; 
class PostProcess;
class ScriptManager;
class UI;

class ENGINE_API Engine
{
public:
	void Init();
	void Destroy() const; 

	static void SetInstance(Engine* newInstance);
	static Engine* GetInstance();

	RenderInterface* GetRenderInterface() const;
	ResourceManager* GetResourceManager() const;
	ImGuiEngine* GetImGuiEngine() const;
	InputManager* GetInputManager() const;
	PhysicsEngine* GetPhysicsEngine() const; 
	AudioEngine* GetAudioEngine() const;
	RenderDebugger* GetRenderDebugger() const;
	RenderTextManager* GetTextManager() const;
	GameStateManager* GetGameStateManager() const; 
	PostProcess* GetPostProcess() const;
	ScriptManager* GetScriptsManager() const;
	UI* GetUI() const;

private:
	static Engine* instance;

	RenderInterface* renderInterface = nullptr; 
	ResourceManager* resourceManager = nullptr; 
	RenderDebugger* renderDebugger = nullptr;
	PhysicsEngine* physicsEngine = nullptr;
	AudioEngine* audioEngine = nullptr;
	InputManager* inputManager = nullptr;
	ImGuiEngine* imGuiEngine = nullptr;
	RenderTextManager* renderTextManager = nullptr;
	GameStateManager* gameStateManager = nullptr;
	PostProcess* postProcess = nullptr;
	ScriptManager* scriptManager = nullptr;
	UI* ui = nullptr;
};