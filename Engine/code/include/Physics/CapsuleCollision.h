#pragma once
#include "CollisionShape.h"

#include "Math/Math.h"

class CapsuleCollision : public  CollisionShape
{
public:
	CapsuleCollision(); 
	CapsuleCollision(float _radius, float _height) : radius(_radius), height(_height) {}

	JPH::ShapeRefC CreateShape() const override;
	void DisplayComponentInInspector(InspectorUI* inspector) override;

	void DrawDebug(const Vector3D& pos, const Vector3D& scale); 

	JPH::ShapeRefC GetJoltShape() const override { return joltShape; }

	bool debug = false; 
	float radius = 1.f;
	float height = 1.f;

private:
	mutable JPH::ShapeRefC joltShape = nullptr;
};