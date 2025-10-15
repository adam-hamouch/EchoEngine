#include "Enemy.h"
#include "Spawner.h"
#include "PlayerController.h"
#include "GameManager.h"

REGISTER_SCRIPT(Enemy)

void Enemy::Start()
{
	player = GetGameObjectByName("Player");
    gameManager = GetGameObjectByName("GameManager")->GetComponent<ScriptComponent>()->GetScript<GameManager>();
}

void Enemy::Update()
{
    const float stoppingDistance = 1.0f;

    Vector3D toPlayer = player->transform.pos - gameObject->transform.pos;
    float distance = toPlayer.Norm();

    if (distance > stoppingDistance)
    {
        direction = Vector3D(toPlayer.Normalize().x, 0, toPlayer.Normalize().z) * speed;
    }
    if (!gameManager->isPaused)
        gameObject->GetComponent<RigidBody>()->SetLinearVelocity(direction);
}

void Enemy::OnCollisionEnter(RigidBody* other)
{
    if (other->GetOwner()->tag == "Bullet")
    {
        Destroy(gameObject);
        Destroy(other->GetOwner());
        gameObject = nullptr;
        Spawner* instance = spawner->GetComponent<ScriptComponent>()->GetScript<Spawner>();
        instance->currentEnemies--;
        player->GetComponent<ScriptComponent>()->GetScript<PlayerController>()->score += 50;
    }
}
