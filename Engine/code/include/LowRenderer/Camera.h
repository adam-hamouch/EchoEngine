#pragma once
#include "Math/Math.h"
#include "Core/DataStructure/Component.h"
#include "Utils/Engine.h"

class InputManager; 
class UI;

class Camera : public Component
{
public:
    ENGINE_API Camera(); 
    ENGINE_API Camera(float _fovY, float _aspectRation, float _near, float _far);
    ~Camera() override = default;

    void Update(float deltaTime);
    ENGINE_API void UpdateCameraGame();

    void SetViewMatrix();
    void SetProjectionMatrix(float fovY, float aspect, float near, float far);
    void SetOrthogonalMatrix(float left, float right, float bottom, float top, float far, float near);
    void SetViewProjectionMatrix();

    Mat4 GetViewMatrix() const { return viewMatrix; }
    Mat4 GetProjectionMatrix() const { return projectionMatrix; }
    Mat4 GetViewProjectionMatrix() const { return viewProjectionMatrix; }
    Mat4 GetOrthogonalMatrix() const { return orthogonalMatrix; }

    Vector3D eye = { 0.f, 0.f, 3.0f };
    Vector3D center = { 0.f, 0.f, -2.f };
    Vector3D up = { 0.f, 1.f, 0.f };
    Vector3D offset = { 0.0f, 1.8f, 0.0f };
    Vector3D forward;
    Vector3D f, r, u;

private:
    void Input(float deltaTime);
    void Rotate(float deltaTime);
    void Move(float deltaTime);
    void DisplayComponentInInspector(InspectorUI* inspector) override;

    Mat4 viewMatrix = Mat4::Identity();
    Mat4 projectionMatrix = Mat4::Identity();
    Mat4 orthogonalMatrix = Mat4::Identity();
    Mat4 viewProjectionMatrix = Mat4::Identity();

    double mouseX = 0.0;
    double mouseY = 0.0;

    int lastWidth = 0;
	int lastHeight = 0;

    float worldPitch = 0;
    float worldYaw = 0;

    float movementSpeed = 20.f;
    float sensitivity = 1.f;

    float m_near;
    float m_far;
    float aspectRatio;
    float fovY;

    bool isRightMouseHeld = false;
    bool wasRightMouseDown = false;
    
    bool isMoving = false; 

    Vector2D oldPos;

    InputManager* inputManager = nullptr;
    UI* ui = nullptr;
};