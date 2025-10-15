#pragma once

class Component; 

class ComponentUI
{
public:
	virtual ~ComponentUI() = default;
	virtual void Draw(Component* component) = 0;
};