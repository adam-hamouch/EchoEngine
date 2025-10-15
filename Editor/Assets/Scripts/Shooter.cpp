#include "Shooter.h"
#include "GameManager.h"

REGISTER_SCRIPT(Shooter)

void Shooter::Start()
{
	bulletPrefab = GetGameObjectByName("Bullet");
    gameManager = GetGameObjectByName("GameManager")->GetComponent<ScriptComponent>()->GetScript<GameManager>();
    camera = GetGameObjectByName("Camera");
}

void Shooter::Update()
{
    cooldownTimer += inputs->GetDeltaTime();

    Transform transform;
    transform.pos = gameObject->globalTransform.pos + camera->GetComponent<Camera>()->forward;
    transform.scale = bulletPrefab->transform.scale;

    if (inputs->GetMouseButtonDown(MouseCode::MouseButton1) && cooldownTimer >= cooldown && !gameManager->isPaused)
    {
        Instantiate(bulletPrefab, transform);
        cooldownTimer = 0.0f; 
    }
}