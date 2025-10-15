#pragma once
#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "GameBehaviour/EchoStandard.h"

class GameManager;

class PlayerController : public MonoBehaviour
{
public:
    void Start() override;
    void Update() override;

    GameObject* healthText;
    GameObject* scoreText;
    GameObject* gameManager;
    GameObject* defeat;
    GameManager* managerComponent;

    Vector3D direction;
    float speed = 8.0f;
    float baseSpeed = 8.0f;
    float runSpeed = 12.0f;
    float mouseSensitivity = 1.f;
    int score = 0;

    int health = 5;
    bool isDead = false;

    void Move();
    void Death();
    void TakeDamage(float damage);
    void OnCollisionEnter(RigidBody* other) override;
};