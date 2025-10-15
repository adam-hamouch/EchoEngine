#include "Core/UI/Windows/HierarchyUI.h"
#include "Core/Engine.h"
#include "Core/DataStructure/GameObject.h"

#include "Resources/SceneGraph.h"
#include "Resources/ResourceManager.h"

#include "LowRenderer/Camera.h"

#include "GameBehaviour/GameStateManager.h"

#include <imgui.h>

HierarchyUI::HierarchyUI()
{
	gameStateManager = Engine::GetInstance()->GetGameStateManager();
	resourceManager = Engine::GetInstance()->GetResourceManager(); 
}

void HierarchyUI::Render()
{
	if (ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse))
	{
		AddGameObjects();

		ImGui::CollapsingHeader("Scene");

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Object");
			if (payload)
			{
				GameObject* data = *(GameObject**)payload->Data;
				gameStateManager->GetCurrentScene()->ChangeParent(gameStateManager->GetCurrentScene()->GetWorld(), data);
			}
			ImGui::EndDragDropTarget();
		}
		auto& objs = gameStateManager->GetCurrentScene()->GetObjects();
		for (auto& obj : objs)
		{
			if (obj->parent->name == "Root")
			{
				SelectNode(obj);
			}
		}
	}

	ImGui::End();
}

void HierarchyUI::HierarchyChildren(GameObject* obj)
{
	for (auto& child : obj->children)
	{
		SelectNode(child);
	}
}

void HierarchyUI::SelectNode(GameObject* object)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (selectedObject == object)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	if (object->children.empty())
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	bool open = ImGui::TreeNodeEx(object->name.c_str(), nodeFlags);

	if (ImGui::IsItemClicked())
	{
		selectedObject = object;
	}

	DragAndDrop(object);

	if (open)
	{
		HierarchyChildren(object);
		ImGui::TreePop();
	}
}

void HierarchyUI::DragAndDrop(GameObject* object)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::SetDragDropPayload("Object", &selectedObject, sizeof(GameObject*), ImGuiCond_Once);
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Object");
		if (payload)
		{
			GameObject* data = *(GameObject**)payload->Data;
			gameStateManager->GetCurrentScene()->ChangeParent(object, data);
		}
		ImGui::EndDragDropTarget();
	}
}

void HierarchyUI::AddGameObjects()
{
	ImGui::Indent(75.f);

	if (ImGui::Button("+"))
	{
		showCreateWindow = true;
		ImGui::SetNextWindowPos(ImGui::GetMousePos());
	}

	if (showCreateWindow)
	{
		ImGui::Begin("Create GameObjects", &showCreateWindow, ImGuiWindowFlags_NoCollapse);

		if (ImGui::Button("Create Empty"))
			CreateEmpty();

		if (ImGui::Button("Create Sphere"))
			CreateSphere();

		if (ImGui::Button("Create Cube"))
			CreateCube();

		if (ImGui::Button("Create Camera"))
			CreateCamera();

		ImGui::End();  
	}

	ImGui::Unindent(75.f);
}

void HierarchyUI::CreateEmpty()
{
	gameStateManager->GetCurrentScene()->CreateGameObject("Empty GameObject");
	showCreateWindow = false;
}

void HierarchyUI::CreateSphere()
{
	gameStateManager->GetCurrentScene()->CreateGameObject("Sphere", resourceManager->Get<IModel>("sphere.obj"), resourceManager->Get<IMaterial>("default.mat"), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f));
	showCreateWindow = false;
}

void HierarchyUI::CreateCube()
{
	gameStateManager->GetCurrentScene()->CreateGameObject("Cube", resourceManager->Get<IModel>("cube.obj"), resourceManager->Get<IMaterial>("default.mat"), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f));
	showCreateWindow = false;
}

void HierarchyUI::CreateCamera()
{
	GameObject* camera = new GameObject();
	camera->name = "Camera";
	camera->AddComponent<Camera>();
	gameStateManager->GetCurrentScene()->AddChildren(gameStateManager->GetCurrentScene()->GetWorld(), camera);
	gameStateManager->GetCurrentScene()->GetObjects().push_back(camera);
}
