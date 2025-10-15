#include "Core/Engine.h"
#include "Core/Inputs/InputManager.h"

#include "GameBehaviour/GameStateManager.h"

#include "Physics/RenderDebugger.h"
#include "Physics/PhysicsEngine.h"

#include "Sound/AudioEngine.h"

#include "Resources/ResourceManager.h"

#include "LowRenderer/LightEngine.h"
#include "LowRenderer/RenderTextManager.h"

#include "RenderInterface/RHI.h"
#include "RenderInterface/OpenGL.h"

#include "Core/UI/ImGuiEngine.h"
#include "Core/UI/UI.h"

#include "GameBehaviour/ScriptsManager.h"

Engine* Engine::instance = nullptr;

void Engine::Init()
{
	gameStateManager = new GameStateManager(); 
	renderTextManager = new RenderTextManager(); 
	imGuiEngine = new ImGuiEngine(); 
	inputManager = new InputManager(); 
	renderInterface = new RenderInterfaceGL(); 
	resourceManager = new ResourceManager(); 
	renderDebugger = new RenderDebugger();
	physicsEngine = new PhysicsEngine();
	audioEngine = new AudioEngine();
	scriptManager = new ScriptManager();
	ui = new UI();
}

void Engine::Destroy() const
{
	physicsEngine->Shutdown(); 
	audioEngine->Shutdown();

	delete gameStateManager; 
	delete renderTextManager; 
	delete imGuiEngine;
	delete inputManager; 
	delete resourceManager; 
	delete renderDebugger; 
	delete physicsEngine; 
	delete audioEngine; 
	delete scriptManager;
}

void Engine::SetInstance(Engine* newInstance)
{
	instance = newInstance;
}

Engine* Engine::GetInstance()
{
	return instance;
}

RenderInterface* Engine::GetRenderInterface() const
{
	return renderInterface;
}

ResourceManager* Engine::GetResourceManager() const
{
	return resourceManager;
}

InputManager* Engine::GetInputManager() const
{
	return inputManager;
}

RenderDebugger* Engine::GetRenderDebugger() const
{
	return renderDebugger;
}

PhysicsEngine* Engine::GetPhysicsEngine() const
{
	return physicsEngine;
}

AudioEngine* Engine::GetAudioEngine() const
{
	return audioEngine;
}

RenderTextManager* Engine::GetTextManager() const
{
	return renderTextManager;
}

GameStateManager* Engine::GetGameStateManager() const
{
	return gameStateManager;
}

PostProcess* Engine::GetPostProcess() const
{
	return postProcess;
}

ScriptManager* Engine::GetScriptsManager() const
{
	return scriptManager;
}

UI* Engine::GetUI() const
{
	return ui;
}

ImGuiEngine* Engine::GetImGuiEngine() const
{
	return imGuiEngine;
}
