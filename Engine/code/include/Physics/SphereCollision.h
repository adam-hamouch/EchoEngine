#pragma once
#include "CollisionShape.h"

#include "Math/Math.h"

class SphereCollision : public CollisionShape
{
public:
    SphereCollision();
    SphereCollision(float _radius) : radius(_radius) {}

	JPH::ShapeRefC CreateShape() const override;
    void DisplayComponentInInspector(InspectorUI* inspector) override;

    void DrawDebug(const Vector3D& pos, const Vector3D& scale) const;

	JPH::ShapeRefC GetJoltShape() const override { return joltShape; }

    bool debug = false;
    float radius = 1.f;

private:
	mutable JPH::ShapeRefC joltShape = nullptr;
};