#pragma once
#include "Math/Math.h"
#include "Core/DataStructure/Component.h"

class LightGL;

enum class LightTypes
{
	DIRECTIONAL = 0,
	POINT = 1,
	SPOT = 2
};

class ILight : public Component
{
public:
	virtual ~ILight() = default;
	virtual LightGL* GetLightGL() = 0;
	virtual LightTypes GetType() = 0; 
};