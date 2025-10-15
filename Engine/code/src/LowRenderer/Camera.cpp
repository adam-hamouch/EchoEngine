#include "LowRenderer/Camera.h"

#include <algorithm>

#include "Core/Inputs/InputManager.h"
#include "Core/Engine.h"
#include "Core/DataStructure/GameObject.h"
#include "Core/UI/UI.h"

Camera::Camera()
{
    inputManager = Engine::GetInstance()->GetInputManager();
    ui = Engine::GetInstance()->GetUI();

    fovY = 45.f; 
    aspectRatio = 16.f / 9.f; 
    m_near = 0.1f;
    m_far = 400.f; 
}

Camera::Camera(float _fovY, float _aspectRatio, float _near, float _far) : fovY(_fovY), aspectRatio(_aspectRatio), m_near(_near), m_far(_far)
{
    inputManager = Engine::GetInstance()->GetInputManager();
	ui = Engine::GetInstance()->GetUI();
}

void Camera::Input(float deltaTime)
{
    Move(deltaTime); 
    Rotate(deltaTime);
}

void Camera::Rotate(float deltaTime)
{
    bool isRightMouseDown = inputManager->GetMouseButtonDown(MouseCode::MouseButtonRight);

    if (isRightMouseDown && !wasRightMouseDown)
    {
        oldPos = inputManager->GetMousePos();
        isRightMouseHeld = true;
    }
    else if (isRightMouseDown && wasRightMouseDown)
    {
        Vector2D mousePos = inputManager->GetMousePos();
        Vector2D deltaPos = mousePos - oldPos;
        oldPos = mousePos;

        mouseX = deltaPos.x;
        mouseY = deltaPos.y;

        float pitch = static_cast<float>(mouseY) * sensitivity * deltaTime;
        float yaw = static_cast<float>(mouseX) * sensitivity * deltaTime;

        worldPitch = std::clamp(worldPitch + pitch, DegToRad(-45.f), DegToRad(45.f));
    	worldYaw += yaw;

        Mat4 rotationMatrix = Mat4::RotationX(worldPitch) * Mat4::RotationY(worldYaw); 
        f = Mat4::TransformDirection(rotationMatrix, Vector3D(0, 0, -1));

        center = eye + f;
    }

    if (!isRightMouseDown && wasRightMouseDown)
    {
        isRightMouseHeld = false;
    }

    wasRightMouseDown = isRightMouseDown; 
}

void Camera::SetProjectionMatrix(float fovY, float aspect, float near, float far)
{
    Mat4 projection{
    1.f / (aspect * tanf(fovY / 2.f)), 0.f, 0.f, 0.f,
    0.f, 1.f / (tanf(fovY / 2.f)), 0.f, 0.f,
    0.f, 0.f, -1.f * (far + near) / (far - near), -1.f * (2.f * far * near) / (far - near),
    0.f, 0.f, -1.f, 0.f
    };

	projectionMatrix = projection;
}

void Camera::SetOrthogonalMatrix(float left, float right, float bottom, float top, float far, float near)
{
    Mat4 orthogonal{
    2.0f / (right - left), 0.0f,                     0.0f,                    0.0f,
    0.0f,                     2.0f / (top - bottom), 0.0f,                    0.0f,
    0.0f,                     0.0f,                   -2.0f / (far - near),   0.0f,
    -(right + left) / (right - left),
    -(top + bottom) / (top - bottom),
    -(far + near) / (far - near),
    1.0f
    };

	orthogonalMatrix = orthogonal;
}

void Camera::SetViewMatrix()
{
    f = Vector3D::Normalize(center - eye);
    r = Vector3D::Normalize(Vector3D::CrossProduct(f, up));
    u = Vector3D::CrossProduct(r, f);

    viewMatrix = {
        r.x, r.y, r.z, -r.DotProduct(eye),
        u.x, u.y, u.z, -u.DotProduct(eye),
        -f.x, -f.y, -f.z, f.DotProduct(eye),
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

void Camera::SetViewProjectionMatrix()
{
    viewProjectionMatrix = projectionMatrix * viewMatrix;;
}

void Camera::Move(float deltaTime)
{
    if (ImGui::GetIO().MouseWheel > 0 || inputManager->GetKeyDown(Keycode::UpArrow) || inputManager->GetKeyDown(Keycode::W)) {
        center += f * deltaTime * movementSpeed;
        eye += f * deltaTime * movementSpeed;
    }
    if (ImGui::GetIO().MouseWheel < 0 || inputManager->GetKeyDown(Keycode::DownArrow) || inputManager->GetKeyDown(Keycode::S)) {
        center -= f * deltaTime * movementSpeed;
        eye -= f * deltaTime * movementSpeed;
    }
    if (inputManager->GetKeyDown(Keycode::RightArrow) || inputManager->GetKeyDown(Keycode::D)) {
        center += r * deltaTime * movementSpeed;
        eye += r * deltaTime * movementSpeed;
    }
    if (inputManager->GetKeyDown(Keycode::LeftArrow) || inputManager->GetKeyDown(Keycode::A)) {
        center -= r * deltaTime * movementSpeed;
        eye -= r * deltaTime * movementSpeed;
    }
    if (inputManager->GetKeyDown(Keycode::Shift))
    {
        center -= Vector3D(0.f, 1.f, 0.f) * deltaTime * movementSpeed;
        eye -= Vector3D(0.f, 1.f, 0.f) * deltaTime * movementSpeed;
    }
    if (inputManager->GetKeyDown(Keycode::Space))
    {
        center += Vector3D(0.f, 1.f, 0.f) * deltaTime * movementSpeed;
        eye += Vector3D(0.f, 1.f, 0.f) * deltaTime * movementSpeed;
    }
}

void Camera::DisplayComponentInInspector(InspectorUI* inspector)
{
    inspector->CameraComponent();
}

void Camera::Update(float deltaTime)
{
    Input(deltaTime);

    int uiWidth = ui->GetWidthScene();
	int uiHeight = ui->GetHeightScene();

    if (uiWidth != lastWidth || uiHeight != lastHeight)
    {
        aspectRatio = static_cast<float>(uiWidth) / static_cast<float>(uiHeight);

		SetProjectionMatrix(fovY, aspectRatio, m_near, m_far);

        float left = 0.0f;
        float right = static_cast<float>(uiWidth);
        float bottom = 0.0f;
        float top = static_cast<float>(uiHeight);
        SetOrthogonalMatrix(left, right, bottom, top, -1, 1);

        SetViewProjectionMatrix();
    }

    SetViewMatrix();
}

void Camera::UpdateCameraGame()
{
    Mat4 rotationMatrix = Mat4::RotationZ(gameObject->parent->transform.eulerRot.z) * Mat4::RotationY(gameObject->parent->transform.eulerRot.y) * Mat4::RotationX(gameObject->parent->transform.eulerRot.x);
    Vector3D worldOffset = rotationMatrix * offset;

    eye = gameObject->parent->transform.pos + worldOffset;

    forward = rotationMatrix * Vector3D(0.0f, 0.0f, -1.0f);

    center = eye + forward;

    up = rotationMatrix * Vector3D(0.0f, 1.0f, 0.0f);

    int uiWidth = ui->GetWidthScene();
    int uiHeight = ui->GetHeightScene();

    if (uiWidth != lastWidth || uiHeight != lastHeight)
    {
        aspectRatio = static_cast<float>(uiWidth) / static_cast<float>(uiHeight);

        SetProjectionMatrix(fovY, aspectRatio, m_near, m_far);

        float left = 0.0f;
        float right = static_cast<float>(uiWidth);
        float bottom = 0.0f;
        float top = static_cast<float>(uiHeight);
        SetOrthogonalMatrix(left, right, bottom, top, -1, 1);

        SetViewProjectionMatrix();
    }

    SetViewMatrix();
}