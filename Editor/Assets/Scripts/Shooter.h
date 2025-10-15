#pragma once
#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "GameBehaviour/EchoStandard.h"

class GameManager;

class Shooter : public MonoBehaviour
{
public:
    void Start() override;
    void Update() override;
    
    GameObject* bulletPrefab;
    GameManager* gameManager;
    GameObject* camera;
    float cooldown = 0.5f;   
    float cooldownTimer = 0.0f;
};