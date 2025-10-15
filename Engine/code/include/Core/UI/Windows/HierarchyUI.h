#pragma once

class GameObject;
class Camera;
class GameStateManager;
class ResourceManager;

class HierarchyUI
{
public:
    HierarchyUI(); 
    ~HierarchyUI() = default;

    void Render();

    GameObject* GetCurrentObject() const { return selectedObject;  }
    void ClearCurrentObject() { selectedObject = nullptr;  }

private:
    void HierarchyChildren(GameObject* obj); 
    void SelectNode(GameObject* object);
    void DragAndDrop(GameObject* object);

    void AddGameObjects(); 
    void CreateEmpty();
    void CreateSphere();
    void CreateCube();
    void CreateCamera(); 

    GameObject* selectedObject = nullptr;

    bool showCreateWindow = false; 

    GameStateManager* gameStateManager = nullptr;
    ResourceManager* resourceManager  = nullptr; 
};
