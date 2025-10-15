#include "Application.h"

#include "Resources/ResourceManager.h"

#include "Core/Window/IWindow.h"
#include "Sound/AudioEngine.h"
#include "Physics/PhysicsEngine.h"

#include "LowRenderer/RenderTextManager.h"
#include "LowRenderer/Camera.h"

#include "Physics/RenderDebugger.h"

Application::Application()
{
	Engine::SetInstance(new Engine());
	engine = Engine::GetInstance();
	engine->Init();
	profiler = new ProfilerGL();

	scriptManager = engine->GetScriptsManager();
	imGuiEngine = engine->GetImGuiEngine();
	stateManager = engine->GetGameStateManager();
	mainWindow = engine->GetRenderInterface()->InstanciateWindow();
	ui = engine->GetUI();
	camera = new Camera(45, 16.0f / 9.0f, 0.1f, 400.0f);
}

Application::~Application()
{
	CleanUp();
}

void Application::Run()
{
	//Window
	InitWindow();

	//ScriptManager
	scriptManager->ReloadGame();

	//imGui Init
	imGuiEngine->Init(mainWindow);

	//Sound init
	engine->GetAudioEngine()->Init();

	//Physics Init
	engine->GetPhysicsEngine()->Init();

	//Debug Render Init
	engine->GetRenderDebugger()->SetCamera(camera);

	//TextManager Init
	engine->GetTextManager()->LoadFont("Assets/Fonts/test.ttf"); 
	engine->GetTextManager()->LoadBuffer();

	//LoadResources
	engine->GetResourceManager()->LoadResources();

	skybox = new Skybox(engine->GetResourceManager()->Get<IShader>("skybox_shader.json"));

	RenderLoop();
}

void Application::InitWindow()
{
	mainWindow->CreateWindow("Editor");
}

void Application::RenderLoop()
{
	while (!mainWindow->WindowShouldClose())
	{
		engine->GetTextManager()->SetProjection(camera->GetOrthogonalMatrix());

		ui->ManageMainWindow("Echo Engine");
		ui->ManageMenu();

		stateManager->ManageStates(camera, ui);
		skybox->Draw(camera);
		stateManager->UpdateScene(camera, ui, profiler);

		ui->End();

		ui->ManagePerformanceWindow(profiler);
		ui->ManageContentBrowser();
		ui->ManageInspector();
		ui->ManageHierarchy();
		ui->ManageLogWindow();

		ui->Render();

		mainWindow->Update();
	}
}

void Application::CleanUp()
{
	engine->GetResourceManager()->SaveResources("Config/editor.json");

	imGuiEngine->Destroy();
	mainWindow->Destroy();

	//Change 
	engine->GetRenderInterface()->DestroyWindow(mainWindow);

	//Engine::GetInstance()->Destroy();

	delete engine->GetRenderInterface();

	delete ui;
	delete camera;
	delete stateManager;
}
