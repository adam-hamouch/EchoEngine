#include "Spawner.h"
#include "GameManager.h"
#include "Enemy.h"

REGISTER_SCRIPT(Spawner)

void Spawner::Start()
{
	enemyPrefab = GetGameObjectByName("Enemy");
    gameManager = GetGameObjectByName("GameManager")->GetComponent<ScriptComponent>()->GetScript<GameManager>();
	waves = 1;
	currentWave = 1;
	enemyPerWaves = 1;
	currentEnemies = enemyPerWaves;

	waveTimer = timeBetweenWaves;
	spawningWave = false;

	SpawnEnemy();
}

void Spawner::Update()
{
    if (currentWave > waves && currentEnemies == 0)
    {
        gameManager->spwanersDestroyed++;
        gameObject->isActive = false;
        return;
    }
    
    if (!spawningWave)
    {
        if (currentEnemies == 0)
        {
            waveTimer -= inputs->GetDeltaTime();
            if (waveTimer <= 0.0f)
            {
                enemyPerWaves *= 2;
                currentEnemies = enemyPerWaves;
                enemiesSpawnedThisWave = 0;
                enemySpawnTimer = 0.0f;
                spawningWave = true;
            }
        }
    }
    else
    {
        enemySpawnTimer -= inputs->GetDeltaTime();
        if (enemySpawnTimer <= 0.0f && enemiesSpawnedThisWave < enemyPerWaves)
        {
            SpawnEnemy();
            enemiesSpawnedThisWave++;
            enemySpawnTimer = timeBetweenEnemies;
        }

        if (enemiesSpawnedThisWave >= enemyPerWaves)
        {
            currentWave++;
            spawningWave = false;
            waveTimer = timeBetweenWaves;
        }
    }
}

void Spawner::SpawnEnemy()
{
	Transform transform;
	transform.pos = gameObject->transform.pos + Vector3D(0, 8, 0);
	transform.eulerRot = enemyPrefab->transform.eulerRot;
	transform.scale = enemyPrefab->transform.scale;
	GameObject* enemy = Instantiate(enemyPrefab, transform);
    Enemy* enemyInstance = enemy->GetComponent<ScriptComponent>()->GetScript<Enemy>();
    enemyInstance->spawner = gameObject;
}
