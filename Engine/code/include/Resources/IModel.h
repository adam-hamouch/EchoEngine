#pragma once
#include "IResource.h"
#include "Math/Math.h"

class ModelGL;

class IModel : public IResource
{
public:
	~IModel() override = default;

	virtual ModelGL* GetGL() = 0;
};