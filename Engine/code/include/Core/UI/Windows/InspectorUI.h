#pragma once

class GameObject;
class Camera;

class RenderInterface;
class ResourceManager;
class ScriptManager;
class GameStateManager; 
class HierarchyUI; 

class InspectorUI
{
public:
    InspectorUI(HierarchyUI* hierarchy);
    ~InspectorUI() = default; 

    void Render();

    void MeshRenderer();
    void MeshFilter();
    void MaterialComponent();
    void RenderModeComponent();
    void LightComponent();
    void DirectionalLightComponent();
    void SpotLightComponent();
    void PointLightComponent();
    void RigidBodyComponent();
    void SphereColliderComponent();
    void BoxColliderComponent();
    void CapsuleColliderComponent();
    void TextComponent();
    void CameraComponent();
    void SoundDisplay();
    void ScriptDisplay();

private:
    void TransformComponent() const;
    void DisplayComponents();
    void AddComponents();
    void AddScriptComponents();

    bool addComponent = false;
    bool showInspector = true;

    GameObject* currentObject = nullptr;

    RenderInterface* renderInterface = nullptr;
    ResourceManager* resourceManager = nullptr;
    ScriptManager* scriptManager = nullptr;
    GameStateManager* gameStateManager = nullptr;

    HierarchyUI* hierarchy; 
};