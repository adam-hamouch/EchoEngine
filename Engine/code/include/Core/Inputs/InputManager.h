#pragma once
#include "Core/Inputs/Inputs.h"

class InputManager : public Inputs
{
public:
	InputManager() = default;
	~InputManager() override = default;

	InputManager* GetInputManager() override { return this; }

	ENGINE_API bool GetKeyDown(Keycode key) override;
	ENGINE_API bool GetKey(Keycode key) override;
	ENGINE_API bool GetKeyUp(Keycode key) override;
	ENGINE_API bool GetMouseButtonDown(MouseCode button) override;
	ENGINE_API bool GetMouseButtonUp(MouseCode button) override;
	ENGINE_API bool GetMouseButtonClick(MouseCode button) override;
	
	ENGINE_API Vector2D GetMousePos() override;
	ENGINE_API Vector2D GetDelta() override;
	ENGINE_API float GetDeltaTime () override;
};