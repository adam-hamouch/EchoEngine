#pragma once
#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "GameBehaviour/EchoStandard.h"

class GameManager;

class Enemy : public MonoBehaviour
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(RigidBody* other) override;

    GameObject* player;
    GameObject* spawner;
    
    GameManager* gameManager;

    float speed = 5.0f;
    Vector3D direction;
};