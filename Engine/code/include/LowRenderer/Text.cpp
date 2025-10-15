#include "Text.h"
#include "RenderTextManager.h"

#include "Core/Engine.h"
#include "Core/UI/Windows/InspectorUI.h"
#include "Core/DataStructure/GameObject.h"

Text::Text()
{
	renderTextManager = Engine::GetInstance()->GetTextManager(); 
}

void Text::Draw() const
{
	if (renderTextManager)
		renderTextManager->Render(text, x, y, scale, color);
}

void Text::DisplayComponentInInspector(InspectorUI* inspector)
{
	inspector->TextComponent(); 
}
