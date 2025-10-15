#include "Physics/PhysicsEngine.h"
#include <Jolt/RegisterTypes.h>

#include <iostream>
#include <cstdarg>

#include "Core/DataStructure/GameObject.h"
#include "Physics/CollisionShape.h"
#include "Physics/RigidBody.h"
#include "GameBehaviour/Behaviour/MonoBehaviour.h"

void PhysicsEngine::Init() 
{
    RegisterDefaultAllocator();
    Factory::sInstance = new Factory();
    RegisterTypes();

    tempAllocator = new TempAllocatorImpl(10 * 1024 * 1024);
    jobSystem = new JobSystemThreadPool(2048, 8, 4);

    broadPhaseLayerInterface = new MyBroadPhaseLayerInterface();
    objectVsBroadPhaseLayerFilter = new MyObjectVsBroadPhaseLayerFilter();
    objectLayerPairFilter = new MyObjectLayerPairFilter();

    physicsSystem = new PhysicsSystem();
    physicsSystem->Init(1024, 128, 2048, 1024, *broadPhaseLayerInterface, *objectVsBroadPhaseLayerFilter, *objectLayerPairFilter);
}

void PhysicsEngine::Update(float deltaTime) const
{
    physicsSystem->Update(deltaTime, 1, tempAllocator, jobSystem);
}

void PhysicsEngine::Shutdown() const
{
    delete physicsSystem;
    delete jobSystem;
    delete tempAllocator;

    delete broadPhaseLayerInterface;
    delete objectVsBroadPhaseLayerFilter;
    delete objectLayerPairFilter;

    UnregisterTypes();
    delete Factory::sInstance;
}

void PhysicsEngine::CollideShapes(const RigidBody* rbA, const RigidBody* rbB, AllHitCollisionCollector<CollideShapeCollector>& collector) const
{
    CollideShapeSettings settings;
    settings.mActiveEdgeMode = EActiveEdgeMode::CollideWithAll;
    settings.mBackFaceMode = EBackFaceMode::CollideWithBackFaces;
    settings.mCollectFacesMode = ECollectFacesMode::NoFaces;

    CollisionDispatch::sCollideShapeVsShape(
        rbA->GetCollisionShape()->GetJoltShape(),
        rbB->GetCollisionShape()->GetJoltShape(),
        Vec3::sOne(),
        Vec3::sOne(),
        rbA->GetWorldTransformJolt(),
        rbB->GetWorldTransformJolt(),
        SubShapeIDCreator(),
        SubShapeIDCreator(),
        settings,
        collector
    );
}

void PhysicsEngine::UpdateCollision(const std::vector<GameObject*>& allObjects)
{
    std::vector<RigidBody*> allBodies;
    allBodies.reserve(allObjects.size());

    for (GameObject* obj : allObjects)
    {
        if (RigidBody* rb = obj->GetComponent<RigidBody>())
        {
            allBodies.push_back(rb);
        }
    }

    std::unordered_map<RigidBody*, std::unordered_set<RigidBody*>> currentContacts;

    const size_t bodyCount = allBodies.size();
    for (size_t i = 0; i < bodyCount; ++i)
    {
        RigidBody* rbA = allBodies[i];
        if (!rbA->HasCollisionCallbacksEnabled()) continue;

        for (size_t j = i + 1; j < bodyCount; ++j)
        {
            RigidBody* rbB = allBodies[j];
            if (!rbB->HasCollisionCallbacksEnabled()) continue;

            AllHitCollisionCollector<CollideShapeCollector> collector;
            CollideShapes(rbA, rbB, collector);

            if (collector.HadHit())
            {
                currentContacts[rbA].insert(rbB);
                currentContacts[rbB].insert(rbA);

                bool aHadB = previousContacts[rbA].count(rbB) > 0;
                bool bHadA = previousContacts[rbB].count(rbA) > 0;

                ScriptComponent* scriptA = rbA->GetOwner()->GetComponent<ScriptComponent>();
                ScriptComponent* scriptB = rbB->GetOwner()->GetComponent<ScriptComponent>();

                if (scriptA)
                {
                    if (!aHadB)
                        scriptA->GetNativeInstance()->OnCollisionEnter(rbB);
                    else
                        scriptA->GetNativeInstance()->OnCollisionStay(rbB);
                }
                
                if (scriptB)
                {
                    if (!bHadA)
                        scriptB->GetNativeInstance()->OnCollisionEnter(rbA);
                    else
                        scriptB->GetNativeInstance()->OnCollisionStay(rbA);
                }
            }
        }
    }

    previousContacts = std::move(currentContacts);
}
