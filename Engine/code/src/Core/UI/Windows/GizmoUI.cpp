#include "Core/UI/Windows/GizmoUI.h"

#include "Math/Math.h"

#include "Core/DataStructure/GameObject.h"
#include "LowRenderer/Camera.h"

void GizmoUI::Render(Camera* camera, GameObject* gameObject)
{
	if (gameObject)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_T))
			currentOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_R))
			currentOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_S))
			currentOperation = ImGuizmo::SCALE;

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		Mat4 globalMatrix = gameObject->GetGlobalMatrix();
		Mat4 gizmoMatrix = globalMatrix.Transpose(); 

		ImGuizmo::Manipulate(
			camera->GetViewMatrix().Transpose().data,
			camera->GetProjectionMatrix().Transpose().data,
			currentOperation, currentMode,
			gizmoMatrix.data); 

		if (ImGuizmo::IsUsing())
		{
			Mat4 localMatrix = gizmoMatrix;
			if (gameObject->parent)
			{
				Mat4 parentGlobal = gameObject->parent->GetGlobalMatrix().Transpose();
				localMatrix = parentGlobal.InvertMatrix() * gizmoMatrix;
			}

			Transform transform;
			ImGuizmo::DecomposeMatrixToComponents(localMatrix.data, &transform.pos.x, &transform.eulerRot.x, &transform.scale.x);

			gameObject->transform = transform;
		}
	}
}
