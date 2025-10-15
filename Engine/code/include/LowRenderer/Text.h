#pragma once
#include <string>
#include "Math/Math.h"

#include "Core/DataStructure/Component.h"

class RenderTextManager; 

class Text : public Component
{
public:
	Text(); 
	~Text() override = default;

	void Draw() const;
	void DisplayComponentInInspector(InspectorUI* inspector) override;

	std::string text = "default";

	float x = 0.f;
	float y = 0.f;
	float scale = 1.f;

	Vector3D color = Vector3D(1.f, 1.f, 1.f);

private:
	RenderTextManager* renderTextManager; 
};
