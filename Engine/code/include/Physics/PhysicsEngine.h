#pragma once
#include "Utils/Engine.h"

#define JPH_IMPLEMENT_RTTI
#define JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
#define JPH_PROFILE_ENABLED
#define JPH_OBJECT_STREAM
#define JPH_DEBUG_RENDERER

#include <unordered_set>

#include "PhysicsLayers.h"
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/JobSystemThreadPool.h>

#include <Physics/CollisionCollector.h>
#include <Jolt/Physics/Collision/CollisionDispatch.h>

using namespace JPH;

class RigidBody;
class GameObject; 

class PhysicsEngine {
public:
	PhysicsEngine() = default;

    ENGINE_API void Init();
    void Update(float deltaTime) const;
    void Shutdown() const;

    ENGINE_API void UpdateCollision(const std::vector<GameObject*>& allObjects);
    
    PhysicsSystem* GetPhysicsSystem() const { return physicsSystem; }

private:
    void CollideShapes(const RigidBody* rbA, const RigidBody* rbB, AllHitCollisionCollector<CollideShapeCollector>& collector) const;

    PhysicsSystem* physicsSystem = nullptr;
    TempAllocatorImpl* tempAllocator = nullptr;
    JobSystemThreadPool* jobSystem = nullptr;

    MyBroadPhaseLayerInterface* broadPhaseLayerInterface = nullptr;
    MyObjectVsBroadPhaseLayerFilter* objectVsBroadPhaseLayerFilter = nullptr;
    MyObjectLayerPairFilter* objectLayerPairFilter = nullptr;

    std::unordered_map<RigidBody*, std::unordered_set<RigidBody*>> previousContacts;
};

