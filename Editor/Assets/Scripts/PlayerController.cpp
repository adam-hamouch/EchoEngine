#include "PlayerController.h"
#include "GameManager.h"

REGISTER_SCRIPT(PlayerController)

void PlayerController::Start()
{
    isDead = false;
    speed = 5.0f;
    health = 5;

    healthText = GetGameObjectByName("Health");
    scoreText = GetGameObjectByName("Score");
    gameManager = GetGameObjectByName("GameManager");
    defeat = GetGameObjectByName("Defeat");
    gameObject->GetComponent<RigidBody>()->ApplyTransformToPhysics(gameObject->transform.pos, gameObject->transform.scale);
}

void PlayerController::Update()
{
    managerComponent = gameManager->GetComponent<ScriptComponent>()->GetScript<GameManager>();
    direction = Vector3D(0.0f, 0.0f, 0.0f);

    if (!managerComponent->isPaused)
    {
        Move();
    }
    
    healthText->GetComponent<Text>()->text = "Health : " + std::to_string(health);
    scoreText->GetComponent<Text>()->text = "Score : " + std::to_string(score);
}

void PlayerController::Move()
{
    Vector2D mouseDelta = inputs->GetDelta();

    gameObject->transform.eulerRot.y -= mouseDelta.x * mouseSensitivity * inputs->GetDeltaTime();
    gameObject->transform.eulerRot.x -= mouseDelta.y * mouseSensitivity * inputs->GetDeltaTime();

    if (gameObject->transform.eulerRot.x > DegToRad(89.0f))
        gameObject->transform.eulerRot.x = DegToRad(89.0f);
    if (gameObject->transform.eulerRot.x < DegToRad(-89.0f))
        gameObject->transform.eulerRot.x = DegToRad(-89.0f);

    Mat4 rotationMatrix =
        Mat4::RotationZ(gameObject->transform.eulerRot.z) *
        Mat4::RotationY(gameObject->transform.eulerRot.y) *
        Mat4::RotationX(gameObject->transform.eulerRot.x);

    Vector3D forward = rotationMatrix * Vector3D(0.0f, 0.0f, -1.0f);
    Vector3D right = rotationMatrix * Vector3D(1.0f, 0.0f, 0.0f);

    forward.y = 0.0f;
    right.y = 0.0f;
    forward.Normalize();
    right.Normalize();

    if (inputs->GetKeyDown(Keycode::LeftShift))
        speed = runSpeed;
    else
        speed = baseSpeed;

    direction = Vector3D(0.0f, 0.0f, 0.0f);

    if (inputs->GetKeyDown(Keycode::W))
        direction = forward * speed;
    if (inputs->GetKeyDown(Keycode::S))
        direction = forward * -speed;
    if (inputs->GetKeyDown(Keycode::D))
        direction = right * speed;
    if (inputs->GetKeyDown(Keycode::A))
        direction = right * -speed;

    gameObject->GetComponent<RigidBody>()->SetLinearVelocity(direction);
}

void PlayerController::Death()
{
    if (health <= 0)
    {
        isDead = true;
        gameObject->isActive = false;
        defeat->isActive = true;
        managerComponent->isPaused = true;
        managerComponent->returnText->isActive = true;
        managerComponent->health->isActive = false;
        managerComponent->score->isActive = false;
        managerComponent->viseur->isActive = false;
    }
}

void PlayerController::TakeDamage(float damage)
{
    health -= damage;
    Death();
}

void PlayerController::OnCollisionEnter(RigidBody* other)
{
    if (other->GetOwner()->tag == "Enemy")
    {
        TakeDamage(1.0f);
    }
}
