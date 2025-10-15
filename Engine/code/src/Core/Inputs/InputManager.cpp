#include "Core/Inputs/InputManager.h"

bool InputManager::GetKeyDown(Keycode key)
{
	return ImGui::IsKeyDown(static_cast<ImGuiKey>(key));
}

bool InputManager::GetKey(Keycode key)
{
	return ImGui::IsKeyPressed(static_cast<ImGuiKey>(key));
}

bool InputManager::GetKeyUp(Keycode key)
{
	return ImGui::IsKeyReleased(static_cast<ImGuiKey>(key));
}

bool InputManager::GetMouseButtonDown(MouseCode button)
{
	return ImGui::IsMouseDown(static_cast<ImGuiMouseButton>(button));
}

bool InputManager::GetMouseButtonUp(MouseCode button)
{
	return ImGui::IsMouseReleased(static_cast<ImGuiMouseButton>(button));
}

bool InputManager::GetMouseButtonClick(MouseCode button)
{
	return ImGui::IsMouseClicked(static_cast<ImGuiMouseButton>(button)); 
}

Vector2D InputManager::GetMousePos()
{
	return Vector2D(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
}

Vector2D InputManager::GetDelta()
{
	return Vector2D(ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y);
}

float InputManager::GetDeltaTime()
{
	return ImGui::GetIO().DeltaTime;
}
