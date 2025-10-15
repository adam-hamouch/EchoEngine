#pragma once
#include <imgui.h>
#include <ImGuizmo.h>

class Camera;
class GameObject; 

class GizmoUI
{
public:
    void Render(Camera* camera, GameObject* gameObject);

private:
    ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE currentMode = ImGuizmo::LOCAL;
};
