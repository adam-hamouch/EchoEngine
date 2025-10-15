#pragma once

class GameStateManager;
class ScriptManager; 

class MenuUI
{
public:
	MenuUI();

	void Render(); 

private:
	GameStateManager* stateManager;
	ScriptManager* scriptManager;
};