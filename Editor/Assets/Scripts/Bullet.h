#pragma once
#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "GameBehaviour/EchoStandard.h"

class Bullet : public MonoBehaviour
{
public:
    void Start() override;
    void Update() override;
private:
    GameObject* camera;
    Vector3D direction = { 0.0f, 0.0f, 1.0f }; 
    float speed = 1.0f;                 
    float lifeTime = 5.0f;                 
    float lifeTimer = 0.0f;
};