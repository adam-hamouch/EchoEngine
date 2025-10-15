#pragma once
#include "ILight.h"

class UiGUI;

class LightGL : public ILight{
public:
	LightGL() = default; 
	~LightGL() override = default;

	LightTypes GetType() override { return LightTypes::DIRECTIONAL ; }

	LightGL* GetLightGL() override { return this; }
	void DisplayComponentInInspector(InspectorUI* inspector) override;
};

class DirectionalLight : public LightGL
{
public:
	Vector3D direction;
	Vector3D color;
	float intensity = 10.f; 
	LightTypes GetType() override { return lightType; }

private:
	LightTypes lightType = LightTypes::DIRECTIONAL; 
};

class PointLight : public LightGL
{
public:
	Vector3D position;
	Vector3D color;
	float intensity = 10.f;
	LightTypes GetType() override { return lightType; }

private:
	LightTypes lightType = LightTypes::POINT; 
};

class SpotLight : public LightGL
{
public:
	Vector3D position;
	Vector3D direction;
	float spotAngle;
	float spotSmoothAngle;
	Vector3D color;
	float intensity = 10.f;
	LightTypes GetType() override { return lightType; }

private:
	LightTypes lightType = LightTypes::SPOT; 
};
