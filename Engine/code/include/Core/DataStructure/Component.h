#pragma once

class InspectorUI;
class GameObject;

class Component
{
public :
	virtual ~Component() = default;

	virtual void DisplayComponentInInspector(InspectorUI* inspector) = 0;

	GameObject* GetGameObject() const { return gameObject; }
	void SetGameObject(GameObject* _gameObject) { gameObject = _gameObject; }

	GameObject* gameObject = nullptr;
};
