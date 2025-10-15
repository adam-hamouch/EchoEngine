#include "GameManager.h"
#include "PlayerController.h"

REGISTER_SCRIPT(GameManager)

void GameManager::Start()
{
	menuPause = GetGameObjectByName("Pause");
	menuPause2 = GetGameObjectByName("Pause2");
	score = GetGameObjectByName("Score");
	health = GetGameObjectByName("Health");
	returnText = GetGameObjectByName("Return");
	viseur = GetGameObjectByName("Viseur");
	victory = GetGameObjectByName("Victory");
	player = GetGameObjectByName("Player");

	GetGameObjectByName("Spawner")->isActive = true;
	GetGameObjectByName("Spawner2")->isActive = true;
	GetGameObjectByName("Spawner3")->isActive = true;
	GetGameObjectByName("Spawner4")->isActive = true;
}

void GameManager::Update()
{
	if (inputs->GetKey(Keycode::P))
	{
		if (!isPaused)
		{
			Pause();
		}
		else
		{
			UnPause();
		}
	}

	if (spwanersDestroyed >= spawnerNumber)
	{
		victory->isActive = true;
		isPaused = true;
		returnText->isActive = true;
		health->isActive = false;
		score->isActive = false;
		viseur->isActive = false;
	}

	if (inputs->GetKey(Keycode::R) && isPaused)
	{
		UnPause();
		OpenScene("TitleScreen.scene");
	}
}

void GameManager::Pause()
{
	menuPause->isActive = true;
	menuPause2->isActive = true;
	isPaused = true;
	health->isActive = false;
	score->isActive = false;
	returnText->isActive = true;
	viseur->isActive = false;
}

void GameManager::UnPause()
{
	returnText->isActive = false;
	menuPause->isActive = false;
	menuPause2->isActive = false;
	isPaused = false;
	health->isActive = true;
	score->isActive = true;
	viseur->isActive = true;
	victory->isActive = false;
	player->GetComponent<ScriptComponent>()->GetScript<PlayerController>()->gameObject->isActive = true;
	player->GetComponent<ScriptComponent>()->GetScript<PlayerController>()->defeat->isActive = false;
}