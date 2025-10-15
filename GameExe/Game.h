#pragma once
#include "Core/UI/UI.h"

#include "Resources/IShader.h"

#include "GameBehaviour/GameStateManager.h"
#include "LowRenderer/Camera.h"
#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "GameBehaviour/ScriptsManager.h"

#include "Resources/IShader.h"

#include "GameBehaviour/GameStateManager.h"
#include "LowRenderer/Camera.h"
#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "LowRenderer/SkyBox.h"

#include "Core/UI/ImGuiEngine.h"
#include "Core/Engine.h"
#include "Profiler/ProfilerGL.h"

class Game
{
public:
	Game();
	~Game();

	void Run();
private:
	IWindow* mainWindow;
	Camera* camera;
	GameStateManager* stateManager;

	SceneGraph* graph;
	MonoBehaviour* behaviour;
	Skybox* skybox;
	PostProcess* postProcess;
	ImGuiEngine* imGuiEngine;

	PerformanceProfiler* profiler;
	UI* ui;

	Engine* engine;
	ScriptManager* scriptManager;

	bool sceneOpened = false;

	void InitWindow();
	void RenderLoop();
	void CleanUp();
};
