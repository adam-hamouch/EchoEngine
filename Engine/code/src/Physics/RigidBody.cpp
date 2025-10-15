#include "Physics/RigidBody.h"

#include "Physics/CollisionShape.h"
#include "Utils/JoltUtils.h"

#include "Core/DataStructure/GameObject.h"
#include "Core/UI/UI.h"

#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include "Core/Engine.h"

RigidBody::RigidBody(CollisionShape* _shape, GameObject* _obj, EMotionType motionType) : shape(_shape), owner(_obj)
{
    physics = Engine::GetInstance()->GetPhysicsEngine()->GetPhysicsSystem();

    Vec3 pos = { owner->transform.pos.x, owner->transform.pos.x, owner->transform.pos.x };

    BodyCreationSettings settings(shape->CreateShape(), pos, Quat::sIdentity(), motionType, 0);

    BodyInterface& bodyInterface = physics->GetBodyInterface();
    bodyID = bodyInterface.CreateAndAddBody(settings, EActivation::Activate);
}

void RigidBody::UpdateShape(const CollisionShape* newShape) const
{
    BodyInterface& bodyInterface = physics->GetBodyInterface();

    ShapeRefC shape = newShape->CreateShape();
    Vector3D objScale = owner->transform.scale;

    Vec3 joltScale = JoltUtils::Convert(objScale);

    if (shape->IsValidScale(joltScale))
    {
        ShapeRefC scaledShape = shape->ScaleShape(joltScale).Get();
        bodyInterface.SetShape(bodyID, scaledShape, false, EActivation::Activate);
    }
    else
    {
        bodyInterface.SetShape(bodyID, shape, false, EActivation::Activate);
    }
}

void RigidBody::UpdateTransformFromPhysics() const
{
    BodyLockRead lock(physics->GetBodyLockInterface(), bodyID);
    if (!lock.Succeeded())
        return;

    const Body& body = lock.GetBody();
    
    Vec3 pos = body.GetPosition();

    if (owner)
        owner->transform.pos = { pos.GetX(), pos.GetY(), pos.GetZ() };
}

void RigidBody::ApplyTransformToPhysics(const Vector3D& _pos, const Vector3D& _scale) const
{
    BodyInterface& bodyInterface = physics->GetBodyInterface();

    std::cout << owner->name << std::endl; 


    bodyInterface.SetPosition(bodyID, JoltUtils::Convert(_pos), EActivation::Activate);
}

void RigidBody::DisplayComponentInInspector(InspectorUI* inspector)
{
    inspector->RigidBodyComponent();
}

EMotionType RigidBody::GetMotionType() const
{
    return physics->GetBodyInterface().GetMotionType(bodyID);
}

int RigidBody::GetCollisionLayer() const
{
    return physics->GetBodyInterface().GetObjectLayer(bodyID);
}

CollisionShape* RigidBody::GetCollisionShape() const
{
    return shape;
}

RMat44 RigidBody::GetWorldTransformJolt() const
{
	BodyLockRead lock(physics->GetBodyLockInterface(), bodyID);
	if (!lock.Succeeded())
		return RMat44::sIdentity();

	const Body& body = lock.GetBody();
	return body.GetWorldTransform();
}

GameObject* RigidBody::GetOwner() const
{
    return owner;
}

void RigidBody::SetMotionType(EMotionType inMotionType, EActivation inActivationMode)
{
    BodyInterface& bodyInterface = physics->GetBodyInterface();

    Vec3 pos = { bodyInterface.GetPosition(bodyID).GetX(), bodyInterface.GetPosition(bodyID).GetY(), bodyInterface.GetPosition(bodyID).GetZ() };

    BodyCreationSettings settings(bodyInterface.GetShape(bodyID), pos, Quat::sIdentity(), inMotionType, 0);

    bodyInterface.RemoveBody(bodyID);
    bodyInterface.DestroyBody(bodyID);

    bodyID = bodyInterface.CreateAndAddBody(settings, inActivationMode);
}


void RigidBody::SetGravity(float gravity) const
{
    BodyInterface& bodyInterface = physics->GetBodyInterface();

    bodyInterface.SetGravityFactor(bodyID, gravity);
}

void RigidBody::SetMass(float mass) const
{
    BodyLockWrite lock(physics->GetBodyLockInterface(), bodyID);

    if (lock.Succeeded()) {
        MotionProperties* motionProperties = lock.GetBody().GetMotionProperties();
        MassProperties massProperties = lock.GetBody().GetShape()->GetMassProperties();
        massProperties.ScaleToMass(mass);
        motionProperties->SetMassProperties(EAllowedDOFs::All, massProperties);
    }
}

void RigidBody::SetScale(Vector3D scale)
{
    BodyInterface& bodyInterface = physics->GetBodyInterface();
    ShapeRefC baseShape = shape->CreateShape();

    if (baseShape->IsValidScale(JoltUtils::Convert(scale)))
    {
        ShapeRefC scaledShape = baseShape->ScaleShape(JoltUtils::Convert(scale)).Get();
        bodyInterface.SetShape(bodyID, scaledShape, false, EActivation::Activate);
    }
}

void RigidBody::SaveBodyState()
{
    BodyLockRead lock(physics->GetBodyLockInterface(), bodyID);
    if (!lock.Succeeded())
        return;

    const Body& body = lock.GetBody();
    physics->SaveBodyState(body, stateRecorder);
}

void RigidBody::RestoreBodyState()
{
    stateRecorder.ResetRead();

    BodyLockWrite lock(physics->GetBodyLockInterface(), bodyID);
    if (!lock.Succeeded())
        return;

    Body& body = lock.GetBody();
    physics->RestoreBodyState(body, stateRecorder);
}

void RigidBody::SetLinearVelocity(Vector3D direction) const
{
    if (physics)
    {
        BodyInterface& bodyInterface = physics->GetBodyInterface();

        Vec3 current_velocity = bodyInterface.GetLinearVelocity(bodyID);
        Vec3 desired_velocity = JoltUtils::Convert(direction);

        if (!desired_velocity.IsNearZero() || current_velocity.GetY() < 0.0f)
            desired_velocity.SetY(current_velocity.GetY());

        Vec3 new_velocity = 0.75f * current_velocity + 0.25f * desired_velocity;

        bodyInterface.SetLinearVelocity(bodyID, new_velocity);
    }
    else {
        std::cout << "Marche stp" << std::endl; 
    }
}
