#include "UiManager.h"

REGISTER_SCRIPT(UiManager)

void UiManager::Start()
{
}

void UiManager::Update()
{
	if (inputs->GetKeyDown(Keycode::A))
	{
		OpenScene("Default.scene");
	}

	if (inputs->GetKeyDown(Keycode::B))
	{
		OpenScene("Map2_1.scene");
	}
}