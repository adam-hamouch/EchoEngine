#pragma once
#include "Math/Math.h"

#include "LowRenderer/Light.h"

class IShader; 

class LightEngine {
public:
	ILight* CreateDirectionalLight(const Vector3D& direction, const Vector3D& color);
	ILight* CreatePointLight(const Vector3D& position, const Vector3D& color);
	ILight* CreateSpotLight(const Vector3D& position, const Vector3D& direction, float spotAngle, float smoothAngle, const Vector3D& color);

	void DeleteLight(ILight* lightToDelete);
	void ClearLights(); 

	void UpdateShaderData() const;

private:
	std::vector<DirectionalLight> dirLights;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
};