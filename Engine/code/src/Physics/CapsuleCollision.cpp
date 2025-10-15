#include "Physics/CapsuleCollision.h"
#include "Core/UI/UI.h"

#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

#include "Core/Engine.h"
#include "Physics/RenderDebugger.h"

CapsuleCollision::CapsuleCollision()
{
    radius = 1.f;
    height = 3.f;
}

JPH::ShapeRefC CapsuleCollision::CreateShape() const
{

    JPH::CapsuleShapeSettings settings(height / 2.0f, radius);
    JPH::ShapeSettings::ShapeResult result = settings.Create();

    if (result.HasError()) {
        return nullptr;
    }

	joltShape = result.Get();
    return result.Get();
}

void CapsuleCollision::DisplayComponentInInspector(InspectorUI* inspector)
{
    inspector->CapsuleColliderComponent();
}

void CapsuleCollision::DrawDebug(const Vector3D& pos, const Vector3D& scale)
{
    if (debug)
        Engine::GetInstance()->GetRenderDebugger()->DrawCapsule(pos, radius, height, scale, Vector4D(1.f, 0.f, 0.f, 1.f)); 
}
