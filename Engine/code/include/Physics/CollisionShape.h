#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>
#include "Core/DataStructure/Component.h"

class PhysicsEngine;

class CollisionShape : public Component
{
public:
    virtual ~CollisionShape() override = default;
	virtual JPH::ShapeRefC CreateShape() const = 0;

	virtual JPH::ShapeRefC GetJoltShape() const = 0;
};