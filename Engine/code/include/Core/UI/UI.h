#pragma once
#include "glad/glad.h"
#include "Math/Math.h"

#include "Utils/Engine.h"

#include "imgui.h"

#include "Core/UI/Windows/ContentBrowserUI.h"
#include "Core/UI/Windows/InspectorUI.h"
#include "Core/UI/Windows/HierarchyUI.h"
#include "Core/UI/Windows/PerformanceUI.h"
#include "Core/UI/Windows/RenderingUI.h"
#include "Core/UI/Windows/GizmoUI.h"
#include "Core/UI/Windows/LogUI.h"
#include "Core/UI/Windows/MenuUI.h"

class Camera;
class Skybox;

class UI 
{
public :
	ENGINE_API UI();
	~UI() = default;

	ENGINE_API void Update(); 
	ENGINE_API void Render();
	ENGINE_API void End(); 
	ENGINE_API void ManageMainWindow(const char* name); 
	ENGINE_API void CreateMainWindow(const char* name); 
	ENGINE_API void CreateSceneWindow(const char* windowName); 
	ENGINE_API void CreateRenderingWindow(); 
	ENGINE_API void DrawFloatingWindow();

	ENGINE_API void ManageHierarchy() const;
	ENGINE_API void ManageContentBrowser() const; 
	ENGINE_API void ManageInspector() const;
	ENGINE_API void ManagePerformanceWindow(PerformanceProfiler* profiler) const;
	ENGINE_API void ManageLogWindow() const;
	ENGINE_API void ManageGizmo(Camera* camera) const;
	ENGINE_API void ManageMenu() const; 

	int GetWidthScene() const { return widthScene; }
	int GetHeightScene() const { return heightScene; }

	int widthScene = 0;
	int heightScene = 0;

	HierarchyUI* hierarchy;

private :
	void ResizeBufferWindow();

	GLuint framebuffer; 
	GLuint texture;
	GLuint depthBuffer;

	GLuint msaaFrameBuffer;
	GLuint msaaTexture;
	GLuint msaaDepthBuffer; 

	Vector2D scenePos = Vector2D(0.f, 0.f);

	ImGuiID dockspace_id;

	ContentBrowserUI* contentBrowser;
	InspectorUI* inspector;
	PerformanceUI* performance;
	RenderingUI* render;
	GizmoUI* guizmo;
	LogUI* log;
	MenuUI* menu;
};
