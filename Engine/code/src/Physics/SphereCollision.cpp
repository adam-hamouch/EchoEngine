#include "Physics/SphereCollision.h"
#include "Core/UI/UI.h"

#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include "Core/Engine.h"
#include "Physics/RenderDebugger.h"

SphereCollision::SphereCollision()
{
    radius = 1.f;
}

JPH::ShapeRefC SphereCollision::CreateShape() const
{
    JPH::SphereShapeSettings settings(radius);
    JPH::ShapeSettings::ShapeResult result = settings.Create();

    if (result.HasError()) {
        return nullptr;
    }

	joltShape = result.Get();
    return result.Get();
}

void SphereCollision::DisplayComponentInInspector(InspectorUI* inspector)
{
    inspector->SphereColliderComponent();
}

void SphereCollision::DrawDebug(const Vector3D& pos, const Vector3D& scale) const
{
    if (debug)
        Engine::GetInstance()->GetRenderDebugger()->DrawSphere(pos, radius, scale, Vector4D(1.f, 0.f, 0.f, 1.f)); 
}
