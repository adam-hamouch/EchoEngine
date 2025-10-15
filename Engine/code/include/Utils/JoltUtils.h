#pragma once

#include "Physics/PhysicsEngine.h"
#include "Math/Math.h"

class JoltUtils
{
public:
	static JPH::Vec3Arg Convert(const Vector3D v) { return JPH::Vec3(v.x, v.y, v.z); }
	static Vector3D Convert(const JPH::Vec3& v) { return Vector3D(v.GetX(), v.GetY(), v.GetZ()); }
};