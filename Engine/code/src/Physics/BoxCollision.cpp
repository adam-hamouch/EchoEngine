#include "Physics/BoxCollision.h"
#include "Core/UI/UI.h"

#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include "Core/Engine.h"
#include "Physics/RenderDebugger.h"

BoxCollision::BoxCollision()
{
    size = Vector3D(1.f, 1.f, 1.f);
}

JPH::ShapeRefC BoxCollision::CreateShape() const
{
    JPH::Vec3 halfExtent(size.x, size.y, size.z);
    JPH::BoxShapeSettings settings(halfExtent);
    JPH::ShapeSettings::ShapeResult result = settings.Create();

    if (result.HasError()) {
        return nullptr;
    }

	joltShape = result.Get();
    return result.Get();
}

void BoxCollision::DisplayComponentInInspector(InspectorUI* inspector)
{
    inspector->BoxColliderComponent();
}

void BoxCollision::DrawDebug(const Vector3D& pos, const Vector3D& scale) const
{
    if (debug)
        Engine::GetInstance()->GetRenderDebugger()->DrawCube(pos, size, scale, Vector4D(1.f, 0.f, 0.f, 1.f));
}
