#pragma once
#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "GameBehaviour/EchoStandard.h"

class Enemy;

class GameManager : public MonoBehaviour
{
public:
    void Start() override;
    void Update() override;

    void Pause();
    void UnPause();

    GameObject* menuPause;
    GameObject* health;
    GameObject* score;
    GameObject* menuPause2;
    GameObject* returnText;
    GameObject* viseur;
    GameObject* victory;
    GameObject* player;

    std::vector<Enemy*> enemies;
    int spwanersDestroyed = 0;
    int spawnerNumber = 4;

    bool isPaused = false;
};