#pragma once

#include "Math/Math.h"
#include "Resources/IResource.h"

class MaterialGL;
class ITexture; 

class IMaterial : public IResource
{
public:
	~IMaterial() override = default;

    virtual void SetAlbedoMap(ITexture* tex) = 0;
    virtual void SetMetallicMap(ITexture* tex) = 0;
    virtual void SetRoughnessMap(ITexture* tex) = 0;
    virtual void SetAOMap(ITexture* tex) = 0;
    virtual void SetNormalMap(ITexture* tex) = 0;

    virtual ITexture* GetAlbedoMap() const = 0;
    virtual ITexture* GetMetallicMap() const = 0;
    virtual ITexture* GetRoughnessMap() const = 0;
    virtual ITexture* GetAOMap() const = 0;
    virtual ITexture* GetNormalMap() const = 0;

    virtual void SetAlbedo(const Vector3D& color) = 0;
    virtual void SetRoughness(float value) = 0;
    virtual void SetMetallic(float value) = 0;
    virtual void SetAO(float value) = 0;

    virtual Vector3D GetAlbedo() const = 0;
    virtual float GetRoughness() const = 0;
    virtual float GetMetallic() const = 0;
    virtual float GetAO() const = 0;

    virtual MaterialGL* GetGL() = 0;
};
