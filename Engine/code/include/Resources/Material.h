#pragma once
#include "IMaterial.h"

class ResourceManager; 

struct MaterialParams
{
	ITexture* albedoMap = nullptr;
	ITexture* metallicMap = nullptr;
	ITexture* roughnessMap = nullptr;
	ITexture* aoMap = nullptr;
	ITexture* normalMap = nullptr;

	Vector3D albedo;
	float metallic;
	float roughness;
	float ao;

	MaterialParams() = default;
	MaterialParams(Vector3D _albedo, float _metallic, float _roughness, float _ao)
		: albedo(_albedo), metallic(_metallic), roughness(_roughness), ao(_ao) {
	}
};

class MaterialGL : public IMaterial
{
public:
	MaterialGL(); 
	~MaterialGL() override = default;

	void LoadResource(const char* path) override;

	void SetAlbedoMap(ITexture* tex) override { materialParams.albedoMap = tex; }
	void SetMetallicMap(ITexture* tex) override { materialParams.metallicMap = tex; }
	void SetRoughnessMap(ITexture* tex) override { materialParams.roughnessMap = tex; }
	void SetAOMap(ITexture* tex) override { materialParams.aoMap = tex; }
	void SetNormalMap(ITexture* tex) override { materialParams.normalMap = tex; }

	ITexture* GetAlbedoMap() const override { return materialParams.albedoMap; }
	ITexture* GetMetallicMap() const override { return materialParams.metallicMap; }
	ITexture* GetRoughnessMap() const override { return materialParams.roughnessMap; }
	ITexture* GetAOMap() const override { return materialParams.aoMap; }
	ITexture* GetNormalMap() const override { return materialParams.normalMap; }

	void SetAlbedo(const Vector3D& color) override { materialParams.albedo = color; }
	void SetMetallic(float value) override { materialParams.metallic = value; }
	void SetRoughness(float value) override { materialParams.roughness = value; }
	void SetAO(float value) override { materialParams.ao = value; }

	Vector3D GetAlbedo() const override { return materialParams.albedo; }
	float GetRoughness() const override { return materialParams.roughness; }
	float GetMetallic() const override { return materialParams.metallic; }
	float GetAO() const override { return materialParams.ao; }

	MaterialGL* GetGL() override { return this; }

private:
	MaterialParams materialParams;

	ResourceManager* resourceManager; 
};