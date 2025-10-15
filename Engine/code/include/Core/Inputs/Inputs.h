#pragma once
#include "Math/Math.h"
#include "Core/Inputs/KeyCode.h"

class InputManager;

class Inputs
{
public :
	virtual ~Inputs() = default;

	virtual InputManager* GetInputManager() = 0;

	virtual bool GetKeyDown(Keycode key) = 0;
	virtual bool GetKey(Keycode key) = 0;
	virtual bool GetKeyUp(Keycode key) = 0;
	virtual bool GetMouseButtonDown(MouseCode button) = 0;
	virtual bool GetMouseButtonUp(MouseCode button) = 0;
	virtual bool GetMouseButtonClick(MouseCode button) = 0; 

	virtual Vector2D GetMousePos() = 0;
	virtual Vector2D GetDelta() = 0; 
	virtual float GetDeltaTime() = 0;
};

