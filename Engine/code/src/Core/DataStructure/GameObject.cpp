#include "Core/DataStructure/GameObject.h"

GameObject::GameObject()
{
	transform.pos = { 0.0f, 0.0f, 0.0f };
	transform.eulerRot = { 0.0f, 0.0f, 0.0f };
	transform.scale = { 1.0f, 1.0f, 1.0f };
}

GameObject::GameObject(const char* _name, IModel* model, IMaterial* material, Vector3D position, Vector3D rotate, Vector3D scaling, RenderInterface* intf)
{
	IMesh* mesh = intf->InstanciateMesh();
	name = _name;
	mesh->Create(model,material);

	transform.pos = position;
	transform.eulerRot = rotate;
	transform.scale = scaling;

	components.push_back(mesh);
}

void GameObject::SetLocalMatrix()
{
	Vector3D eulerRot(DegToRad(transform.eulerRot.x), DegToRad(transform.eulerRot.y), DegToRad(transform.eulerRot.z));
	localTransform = Mat4::TRS(transform.pos, eulerRot, transform.scale);
}

void GameObject::RemoveComponent(Component* component)
{
	components.erase(std::find(components.begin(), components.end(), component));
}

GameObject* GameObject::FindObjectByName(const std::vector<GameObject*>& objects, const std::string& name)
{
	auto it = std::find_if(objects.begin(), objects.end(),
		[&name](GameObject* obj) {
			return obj->name == name;
		});

	return (it != objects.end()) ? *it : nullptr;
}