#pragma once
#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "GameBehaviour/EchoStandard.h"

class GameManager;

class Spawner : public MonoBehaviour
{
public:
    void Start() override;
    void Update() override;

    void SpawnEnemy();

    int waves = 5;
    int currentWave = 1;
    int enemyPerWaves = 3;
    int currentEnemies = 0;

    float timeBetweenWaves = 3.0f;
    float timeBetweenEnemies = 0.5f;

    float waveTimer = 0.0f;
    float enemySpawnTimer = 0.0f;

    int enemiesSpawnedThisWave = 0;
    bool spawningWave = false;

    GameObject* enemyPrefab;
    GameManager* gameManager;
};