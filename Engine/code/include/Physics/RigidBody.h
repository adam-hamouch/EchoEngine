#pragma once

#include "Core/DataStructure/Component.h"
#include "PhysicsEngine.h"
#include "MemoryStateRecorder.h"
#include "Math/Math.h"

class GameObject;
class CollisionShape;
class UiGUI;

class RigidBody : public Component
{
public:
    BodyID bodyID;

    RigidBody() = default;
    RigidBody(CollisionShape* shape, GameObject* obj, EMotionType motionType);

    void DisplayComponentInInspector(InspectorUI* inspector) override;

    void UpdateTransformFromPhysics() const;
    ENGINE_API void ApplyTransformToPhysics(const Vector3D& pos, const Vector3D& scale) const;

    void UpdateShape(const CollisionShape* newShape) const;
    CollisionShape* GetCollisionShape() const;
    bool GetIsTrigger() const;

    void SetEnableCollisionCallback(bool enabled) { collisionCallbacksEnabled = enabled; }
    bool HasCollisionCallbacksEnabled() const { return collisionCallbacksEnabled; }

    void SetMotionType(EMotionType inMotionType, EActivation inActivationMode);
    void SetGravity(float gravity) const;
    void SetMass(float mass) const;
    void SetScale(Vector3D scale);
    ENGINE_API void SetLinearVelocity(Vector3D direction) const;

    EMotionType GetMotionType() const;
    int GetCollisionLayer() const;
    RMat44 GetWorldTransformJolt() const;
    ENGINE_API GameObject* GetOwner() const;

    void SaveBodyState();
    void RestoreBodyState();

private:
    PhysicsSystem* physics = nullptr;
    GameObject* owner = nullptr;
    CollisionShape* shape = nullptr;

    bool collisionCallbacksEnabled = false;

    MemoryStateRecorder stateRecorder;
};
