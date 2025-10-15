#pragma once
#include "ComponentUI.h"

class MeshRendererUI : public ComponentUI
{
public:
	void Draw(Component* component) override;

private:
	void MeshRenderer();
	void MeshFilter();
	void Material();
};