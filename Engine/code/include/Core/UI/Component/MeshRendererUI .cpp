#include "MeshRendererUI .h"

#include <imgui.h>

#include "LowRenderer/IMesh.h"

//void MeshRendererUI::Draw(Component* component)
//{
//	auto* mesh = dynamic_cast<IMesh*>(component);
//	if (!mesh) return;
//
//	MeshRenderer();
//	MeshFilter();
//	Material();
//}
//
//void MeshRendererUI::MeshRenderer()
//{
//	ImGui::Indent(10.f);
//	if (ImGui::CollapsingHeader("Mesh Renderer"))
//	{
//		ITexture* currentTex = currentObject->GetComponent<IMesh>()->Texture();
//		std::vector<ITexture*> allTextures = resourceManager->GetAllResources<ITexture>();
//
//		ITexture* selectedTex = ImGuiHelper::ResourceSelector("Select a Texture", allTextures, currentTex);
//
//		if (selectedTex != currentTex)
//			currentObject->GetComponent<IMesh>()->Texture(selectedTex);
//
//		if (currentTex)
//			ImGui::Image((ImTextureID)(intptr_t)currentTex->GetTextureID(), ImVec2(128, 128));
//	}
//	ImGui::Unindent(10.f);
//	ImGui::Dummy(ImVec2(0.0f, 10.0f));
//}
//
//void MeshRendererUI::MeshFilter()
//{
//
//}
//
//void MeshRendererUI::Material()
//{
//
//}
