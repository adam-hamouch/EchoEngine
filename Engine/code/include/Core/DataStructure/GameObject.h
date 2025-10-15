#pragma once
#include "Core/DataStructure/Component.h"
#include "Math/Math.h"
#include <vector>

#include "LowRenderer/IMesh.h"
#include "RenderInterface/RHI.h"

class GameObject
{
public:
	GameObject();
	GameObject(const char* _name, IModel* model, IMaterial* material, Vector3D position, Vector3D rotate, Vector3D scaling, RenderInterface* intf);

	~GameObject() = default;

	std::string name = "Root";
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	std::string tag = "Tag";
	Transform transform;
	Transform globalTransform;
	bool isInstantiated = false;
	void SetLocalMatrix();
	void RemoveComponent(Component* component);
	static GameObject* FindObjectByName(const std::vector<GameObject*>& objects, const std::string& name);

	const Mat4& GetLocalMatrix() const { return localTransform; }
	const Mat4& GetGlobalMatrix() const { return globalMatrix; }
	void SetLocalMatrix(const Mat4& matrix) { localTransform = matrix; }
	void SetGlobalMatrix(const Mat4& matrix) { globalMatrix = matrix; }
	std::vector<Component*> components;

	template <typename T>
	void RemoveComponentOfType()
	{
		for (auto it = components.begin(); it != components.end(); ++it)
		{
			if (dynamic_cast<T*>(*it))
			{
				delete* it;
				components.erase(it);
				return;
			}
		}
	}

	template <typename T>
	void SetComponent(Component* newComponent)
	{
		for (Component*& comp : components)
		{
			if (dynamic_cast<T*>(comp))
			{
				delete comp;
				comp = newComponent;
				return;
			}
		}
	}

	template <typename T>
	T* GetComponent()
	{
		for (Component* comp : components)
		{
			if (T* castedComp = dynamic_cast<T*>(comp))
			{
				return castedComp;
			}
		}
		return nullptr;
	}

	template <typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		for (Component* comp : components)
		{
			if (T* castedComp = dynamic_cast<T*>(comp))
			{
				return castedComp; 
			}
		}

		std::cout << "Add component" << std::endl;
		T* component = new T(std::forward<Args>(args)...);  
		component->SetGameObject(this);
		components.push_back(component);
		return component;
	}

	bool isActive = true;
private :
	Mat4 localTransform;
	Mat4 globalMatrix;
};
