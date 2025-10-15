#pragma once
#include "Core/UI/UI.h"

#include "LowRenderer/Camera.h"
#include "LowRenderer/SkyBox.h"

#include "GameBehaviour/ScriptsManager.h"
#include "GameBehaviour/GameStateManager.h"

#include "Resources/SceneGraph.h"

#include "Core/UI/ImGuiEngine.h"
#include "Core/Engine.h"
#include "Profiler/ProfilerGL.h"

class Application
{
public:
	Application();
	~Application();

	void Run();
private:
	IWindow* mainWindow;
	Camera* camera;
	GameStateManager* stateManager;

	ImGuiEngine * imGuiEngine;

	UI* ui;

	SceneGraph* graph;
	Skybox* skybox;

	Engine* engine; 
	PerformanceProfiler* profiler;
	ScriptManager* scriptManager;

	bool sceneOpened = false;

	void InitWindow();
	void RenderLoop();
	void CleanUp();
};
