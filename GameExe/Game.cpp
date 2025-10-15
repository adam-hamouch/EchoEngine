#include "Game.h"

#include "Resources/ResourceManager.h"
#include "Resources/IShader.h"

#include "Core/Window/IWindow.h"
#include "Sound/AudioEngine.h"
#include "Physics/PhysicsEngine.h"

#include "LowRenderer/RenderTextManager.h"
#include "LowRenderer/Camera.h"

#include "GameBehaviour/GameStateManager.h"
#include "Physics/RenderDebugger.h"

Game::Game()
{
	Engine::SetInstance(new Engine());
	engine = Engine::GetInstance();
	engine->Init();
	profiler = new ProfilerGL();

	scriptManager = engine->GetScriptsManager();
	scriptManager->ReloadGame();
	imGuiEngine = engine->GetImGuiEngine();
	stateManager = engine->GetGameStateManager();
	mainWindow = engine->GetRenderInterface()->InstanciateWindow();
	ui = engine->GetUI();
	camera = new Camera(45, 16.0f / 9.0f, 0.1f, 400.0f);
}

Game::~Game()
{
	CleanUp();
}

void Game::Run()
{
	InitWindow();

	scriptManager->ReloadGame();
	imGuiEngine->Init(mainWindow);

	engine->GetAudioEngine()->Init();
	engine->GetPhysicsEngine()->Init();
	engine->GetRenderDebugger()->SetCamera(camera);
	engine->GetTextManager()->LoadFont("Assets/Fonts/test.ttf");
	engine->GetTextManager()->LoadBuffer();
	engine->GetResourceManager()->LoadResources();

	scriptManager->Start(stateManager->currentGraph->GetObjects());

	RenderLoop();
}

void Game::InitWindow()
{
	mainWindow->CreateWindow("Game");
}

void Game::RenderLoop()
{
	while (!mainWindow->WindowShouldClose())
	{
		engine->GetTextManager()->SetProjection(camera->GetOrthogonalMatrix());
		ui->ManageMainWindow("Echo Engine");
		stateManager->state = GameState::PLAYING;

		stateManager->ManageStates(camera, ui);
		stateManager->UpdateScene(camera, ui, profiler);
		stateManager->Render(camera);
		stateManager->state = GameState::PLAYING;
		ui->End();
		ui->Render();
		mainWindow->Update();
	}
}

void Game::CleanUp()
{
	mainWindow->Destroy();

	engine->GetRenderInterface()->DestroyWindow(mainWindow);

	delete engine->GetRenderInterface();

	delete camera;
	delete stateManager;
}
