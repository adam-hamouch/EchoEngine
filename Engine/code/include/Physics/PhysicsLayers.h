#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

// ------------------------------------------
// 1. Layers used by the user
// ------------------------------------------
enum ObjectLayers : JPH::ObjectLayer
{
    SOLID = 0,
    PLAYER = 1,
    TRIGGER = 2,

    NUM_OBJECT_LAYERS
};

// ------------------------------------------
// 2. Layers used by BroadPhase
// ------------------------------------------
enum BroadPhaseLayers : uint8_t
{
    BP_SOLID = 0,
    BP_PLAYER = 1,
    BP_TRIGGER = 2,

    NUM_BROAD_PHASE_LAYERS
};

// ------------------------------------------
// 3. Interface ObjectLayer  BroadPhaseLayer
// ------------------------------------------
class MyBroadPhaseLayerInterface final : public JPH::BroadPhaseLayerInterface
{
public:
    MyBroadPhaseLayerInterface()
    {
        mObjectToBroadPhase[SOLID] = JPH::BroadPhaseLayer(BP_SOLID);
        mObjectToBroadPhase[PLAYER] = JPH::BroadPhaseLayer(BP_PLAYER);
        mObjectToBroadPhase[TRIGGER] = JPH::BroadPhaseLayer(BP_TRIGGER);
    }

    virtual JPH::uint GetNumBroadPhaseLayers() const override
    {
        return NUM_BROAD_PHASE_LAYERS;
    }

    virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
    {
        return mObjectToBroadPhase[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
    {
        switch (inLayer.GetValue())
        {
        case BP_SOLID:  return "BP_SOLID";
        case BP_PLAYER: return "BP_PLAYER";
        case BP_TRIGGER:return "BP_TRIGGER";
        default:        return "INVALID";
        }
    }
#endif

private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[NUM_OBJECT_LAYERS];
};

// ------------------------------------------
// 4. Collision between objects (ObjectLayer/ObjectLayer)
// ------------------------------------------
class MyObjectLayerPairFilter final : public JPH::ObjectLayerPairFilter
{
public:
    virtual bool ShouldCollide(JPH::ObjectLayer a, JPH::ObjectLayer b) const override
    {
        // Implicit symmetry
        if (a == SOLID && b == SOLID)     return true;
        if (a == PLAYER && b == SOLID)    return true;
        if (a == SOLID && b == PLAYER)    return true;
        if (a == PLAYER && b == TRIGGER)  return true;
        if (a == TRIGGER && b == PLAYER)  return true;

        return false;
    }
};

// ------------------------------------------
// 5. Collision between ObjectLayer/BroadPhaseLayer
// ------------------------------------------
class MyObjectVsBroadPhaseLayerFilter final : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
    virtual bool ShouldCollide(JPH::ObjectLayer objLayer, JPH::BroadPhaseLayer broadLayer) const override
    {
        switch (objLayer)
        {
        case SOLID:
            return true; // collide with all

        case PLAYER:
            return broadLayer.GetValue() == BP_SOLID || broadLayer.GetValue() == BP_TRIGGER;

        case TRIGGER:
            return broadLayer.GetValue() == BP_PLAYER;

        default:
            return false;
        }
    }
};