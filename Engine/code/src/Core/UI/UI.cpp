#include "Core/UI/UI.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

UI::UI()
{
	hierarchy = new HierarchyUI();
	contentBrowser = new ContentBrowserUI(hierarchy);
	inspector = new InspectorUI(hierarchy);
	render = new RenderingUI();
	performance = new PerformanceUI();
	guizmo = new GizmoUI();
	log = new LogUI();
	menu = new MenuUI();
}

void UI::Update()
{
	glClearColor(0.53f, 0.81f, 0.92f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuizmo::BeginFrame();
}

void UI::Render()
{
	if (ImGui::GetCurrentContext())
	{
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	else
	{
		std::cerr << "ImGui context is null!" << std::endl;
	}
}

void UI::End()
{
	ImGui::End(); 
}

void UI::ManageMainWindow(const char* name)
{
	Update();
	CreateMainWindow(name);
}

void UI::CreateMainWindow(const char* name)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::Begin(name, nullptr,
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus);

	dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
		ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode
	);
}

void UI::CreateSceneWindow(const char* windowName)
{
	ImGui::Begin(windowName);
	CreateRenderingWindow();

	contentBrowser->DropAsset(); 
}

void UI::CreateRenderingWindow()
{
	ResizeBufferWindow();

	if (widthScene < 1 || heightScene < 1)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
	glViewport(0, 0, widthScene, heightScene);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void UI::DrawFloatingWindow()
{
	if (widthScene < 1 || heightScene < 1)
		return;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glBlitFramebuffer(
		0, 0, widthScene, heightScene,
		0, 0, widthScene, heightScene,
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	render->Render(texture, widthScene, heightScene);
}

void UI::ResizeBufferWindow()
{
	ImVec2 availSize = ImGui::GetContentRegionAvail();

	int newWidth = static_cast<int>(availSize.x);
	int newHeight = static_cast<int>(availSize.y);

	if (newWidth < 1 || newHeight < 1)
		return;

	if (newWidth != widthScene || newHeight != heightScene)
	{
		widthScene = newWidth;
		heightScene = newHeight;

		render->ResizeFramebuffer(widthScene, heightScene,
			framebuffer, texture, depthBuffer,
			msaaFrameBuffer, msaaTexture, msaaDepthBuffer);
	}
}

void UI::ManageHierarchy() const
{
	hierarchy->Render();
}

void UI::ManageInspector() const
{
	inspector->Render();
}

void UI::ManageContentBrowser() const
{
	contentBrowser->Render();
}

void UI::ManagePerformanceWindow(PerformanceProfiler* profiler) const
{
	performance->Render(profiler);
}

void UI::ManageGizmo(Camera* camera) const
{
	guizmo->Render(camera, hierarchy->GetCurrentObject()); 
}

void UI::ManageLogWindow() const
{
	log->Render();
}

void UI::ManageMenu() const
{
	menu->Render(); 
}