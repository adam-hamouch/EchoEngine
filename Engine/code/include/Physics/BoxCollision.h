#pragma once
#include "Math/math.h"
#include "CollisionShape.h"

class BoxCollision : public CollisionShape 
{
public:
	BoxCollision();
	BoxCollision(float width, float height, float depth) : size(width, height, depth) {}

	JPH::ShapeRefC CreateShape() const override;
	void DisplayComponentInInspector(InspectorUI* inspector) override;

	void DrawDebug(const Vector3D& pos, const Vector3D& scale) const;

	JPH::ShapeRefC GetJoltShape() const override { return joltShape; }

	bool debug = false; 
	Vector3D size = Vector3D(0.0f, 0.0f, 0.0f);

private:
	mutable JPH::ShapeRefC joltShape = nullptr;
};