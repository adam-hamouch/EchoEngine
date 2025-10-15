#pragma once
#include "Resources/IResource.h"

#include "fmod.hpp"

class ISound : public IResource
{
public:
    ISound() = default;
    ~ISound() override = default;

    virtual FMOD::Sound* GetInternalSound() const = 0;
};
