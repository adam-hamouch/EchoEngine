#include "Core/UI/Windows/MenuUI.h"
#include "Core/Engine.h"

#include <imgui.h>

#include "Resources/SceneGraph.h"

#include "GameBehaviour/ScriptsManager.h"
#include "GameBehaviour/GameStateManager.h"

MenuUI::MenuUI()
{
	stateManager = Engine::GetInstance()->GetGameStateManager();
	scriptManager = Engine::GetInstance()->GetScriptsManager(); 
}

void MenuUI::Render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				stateManager->GetCurrentScene()->Serialize();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Game"))
		{
			if (stateManager->state == GameState::EDITING)
			{
				if (ImGui::MenuItem("Compile"))
					scriptManager->ReloadGame();

				if (ImGui::MenuItem("Play"))
					stateManager->Play(scriptManager);
			}
			if (stateManager->state == GameState::PLAYING)
			{
				if (ImGui::MenuItem("Stop"))
					stateManager->Stop();

				if (ImGui::MenuItem("Pause"))
					stateManager->Pause();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
