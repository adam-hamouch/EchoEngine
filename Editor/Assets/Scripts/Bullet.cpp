#include "Bullet.h"

REGISTER_SCRIPT(Bullet)

void Bullet::Start()
{
    direction = { 0.0f, 0.0f, 1.0f };
    speed = 50.0f;
    lifeTime = 5.0f;
    lifeTimer = 0.0f;

    GameObject* cam = GetGameObjectByName("Camera");
    if (cam)
    {
        Camera* camComp = cam->GetComponent<Camera>();
        if (camComp)
        {
            direction = camComp->forward.Normalize();
        }
    }
}

void Bullet::Update()
{
    Vector3D velocity = direction * speed;
    gameObject->GetComponent<RigidBody>()->SetLinearVelocity(velocity);
    
    lifeTimer += inputs->GetDeltaTime();
    if (lifeTimer >= lifeTime && gameObject->name != "Bullet")
    {
       Destroy(gameObject);
       gameObject = nullptr;
    }
}