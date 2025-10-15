#include <Core/UI/Windows/InspectorUI.h>

#include <Core/UI/Windows/HierarchyUI.h>

#include <Utils/ImGuiHelper.h>

#include <Core/DataStructure/GameObject.h>

#include <Physics/RigidBody.h>
#include <Physics/BoxCollision.h>
#include <Physics/SphereCollision.h>
#include <Physics/CapsuleCollision.h>

#include <LowRenderer/LightEngine.h>
#include <LowRenderer/Camera.h>
#include <LowRenderer/Text.h>
#include <LowRenderer/IMesh.h>

#include <Resources/ResourceManager.h>

#include <Sound/SoundComponent.h>

#include <Core/Engine.h>
#include "Utils/FileUtils.h"

#include <GameBehaviour/ScriptsManager.h>
#include <GameBehaviour/Behaviour/MonoBehaviour.h>
#include <GameBehaviour/GameStateManager.h>

InspectorUI::InspectorUI(HierarchyUI* _hierarchy) : hierarchy(_hierarchy)
{
	renderInterface = Engine::GetInstance()->GetRenderInterface(); 
	resourceManager = Engine::GetInstance()->GetResourceManager();
	scriptManager = Engine::GetInstance()->GetScriptsManager();
	gameStateManager = Engine::GetInstance()->GetGameStateManager(); 
}

void InspectorUI::Render()
{
	currentObject = hierarchy->GetCurrentObject();

	ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);

	if (showInspector == true && currentObject)
	{
		char bufferName[128];
		strncpy_s(bufferName, currentObject->name.c_str(), sizeof(bufferName));
		bufferName[sizeof(bufferName) - 1] = '\0';

		std::string inputLabel = "##Rename";

		if (ImGui::InputText(inputLabel.c_str(), bufferName, sizeof(bufferName)))
		{
			std::string newname = bufferName;
			if (!newname.empty())
				currentObject->name = bufferName;
		}

		char bufferTag[128];
		strncpy_s(bufferTag, currentObject->tag.c_str(), sizeof(bufferTag));
		bufferTag[sizeof(bufferTag) - 1] = '\0';

		std::string inputTag = "##Tag";

		if (ImGui::InputText(inputTag.c_str(), bufferTag, sizeof(bufferTag)))
		{
			std::string newtag = bufferTag;
			if (!newtag.empty())
				currentObject->tag = bufferTag;
		}

		ImGui::Checkbox("Is Active", &currentObject->isActive);
		TransformComponent();
		DisplayComponents();

		if (ImGui::Button("Delete GameObject"))
		{
			resourceManager->GetAllResources<ILight>(); 

			for (Component* component : currentObject->components)
			{
				if (ILight* light = dynamic_cast<ILight*>(component))
				{
					LightEngine* lightEngine = gameStateManager->currentGraph->GetLightEngine();

					lightEngine->DeleteLight(light);
				}
			}

			gameStateManager->GetCurrentScene()->Delete(currentObject);
			hierarchy->ClearCurrentObject(); 
		}

		AddComponents();
	}

	ImGui::End();
}

void InspectorUI::TransformComponent() const
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("Transform"))
	{
		if (ImGui::InputFloat3("Position", &currentObject->transform.pos.x)) {}
		if (ImGui::InputFloat3("Rotation", &currentObject->transform.eulerRot.x)) {}

		if (ImGui::InputFloat3("Scale", &currentObject->transform.scale.x))
		{
			if (auto* rigidBody = currentObject->GetComponent<RigidBody>())
				rigidBody->SetScale(currentObject->transform.scale);
		}
	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::DisplayComponents()
{
	for (Component* component : currentObject->components)
	{
		ImGui::Separator();

		if (ImGui::Button(("Remove##" + std::to_string(reinterpret_cast<size_t>(component))).c_str()))
		{
			LightEngine* lightEngine = gameStateManager->currentGraph->GetLightEngine();

			if (ILight* light = dynamic_cast<ILight*>(component))
				lightEngine->DeleteLight(light);

			currentObject->RemoveComponent(component);
			break; 
		}

		component->DisplayComponentInInspector(this);
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::AddComponents()
{
	if (ImGui::Button("Add Component"))
	{
		addComponent = true;
		ImGui::SetNextWindowPos(ImGui::GetMousePos());
	}

	if (addComponent)
	{
		ImGui::Begin("Components", nullptr, ImGuiWindowFlags_NoCollapse);

		if (ImGui::Button("Close"))
			addComponent = false;

		if (ImGui::Button("Mesh Component"))
		{
			bool alreadyHasMesh = false;
			for (auto* component : currentObject->components)
			{
				if (dynamic_cast<IMesh*>(component) != nullptr)
				{
					alreadyHasMesh = true;
					break;
				}
			}

			if (!alreadyHasMesh)
			{
				IMesh* mesh = renderInterface->InstanciateMesh();
				mesh->SetMaterial(resourceManager->Get<IMaterial>("default.mat"));
				currentObject->components.push_back(mesh);
			}
		}

		if (ImGui::Button("Collision"))
		{
			ImGui::OpenPopup("CollisionPopup");
		}

		if (ImGui::BeginPopup("CollisionPopup"))
		{
			if (ImGui::Selectable("Sphere"))
			{
				currentObject->AddComponent<SphereCollision>();

				RigidBody* rigidBodyObj = new RigidBody(currentObject->GetComponent<SphereCollision>(), currentObject, JPH::EMotionType::Static);
				currentObject->components.push_back(rigidBodyObj);
			}
			if (ImGui::Selectable("Box"))
			{
				currentObject->AddComponent<BoxCollision>();

				RigidBody* rigidBodyObj = new RigidBody(currentObject->GetComponent<BoxCollision>(), currentObject, JPH::EMotionType::Static);
				currentObject->components.push_back(rigidBodyObj);
			}
			if (ImGui::Selectable("Capsule"))
			{
				currentObject->AddComponent<CapsuleCollision>();

				RigidBody* rigidBodyObj = new RigidBody(currentObject->GetComponent<CapsuleCollision>(), currentObject, JPH::EMotionType::Static);
				currentObject->components.push_back(rigidBodyObj);
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Light"))
		{
			ImGui::OpenPopup("LightPopup");
		}

		if (ImGui::BeginPopup("LightPopup"))
		{
			LightEngine* lightEngine = gameStateManager->currentGraph->GetLightEngine();

			if (ImGui::Selectable("Directional Light"))
			{
				currentObject->components.push_back(
					lightEngine->CreateDirectionalLight(Vector3D(0.f, 0.f, 0.f), Vector3D(0.5f, 0.5f, 0.5f))
				);
			}
			if (ImGui::Selectable("Spot Light"))
			{
				currentObject->components.push_back(
					lightEngine->CreateSpotLight(Vector3D(0.f, 0.f, 0.f), Vector3D(0.f, 0.f, 0.f), 0.f, 0.f, Vector3D(0.5f, 0.5f, 0.5f))
				);
			}
			if (ImGui::Selectable("Point Light"))
			{
				currentObject->components.push_back(
					lightEngine->CreatePointLight(Vector3D(0.f, 0.f, 0.f), Vector3D(0.5f, 0.5f, 0.5f))
				);
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Sound Mixer"))
		{
			currentObject->AddComponent<SoundComponent>();
		}
		if (ImGui::Button("Text"))
		{
			currentObject->AddComponent<Text>();
		}

		if (ImGui::Button("Scripts"))
		{
			ImGui::OpenPopup("ScriptsPopup");
		}

		if (ImGui::BeginPopup("ScriptsPopup"))
		{
			AddScriptComponents();

			ImGui::EndPopup();
		}

		ImGui::End();
	}
}

void InspectorUI::AddScriptComponents()
{
	for (auto& name : scriptManager->scriptNames)
	{
		if (ImGui::Selectable(name.c_str()))
		{
			ScriptComponent* scriptComponent = new ScriptComponent(name, currentObject);

			scriptManager->ReloadGame();
			scriptComponent->Bind(name);

			currentObject->components.push_back(scriptComponent);
		}
	}
}

void InspectorUI::MeshRenderer()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("Mesh Renderer"))
	{
		IMesh* mesh = currentObject->GetComponent<IMesh>();
		IMaterial* currentMaterial = mesh->GetMaterial();

		std::vector<IMaterial*> allMaterials = resourceManager->GetAllResources<IMaterial>();
		IMaterial* selectedMat = ImGuiHelper::ResourceSelector("Select a Material", allMaterials, currentMaterial);

		if (selectedMat && selectedMat != currentMaterial)
			mesh->SetMaterial(selectedMat);

		if (ImGui::Button("Create Material"))
		{
			IMaterial* newMaterial = resourceManager->CreateMaterialFile();
			if (newMaterial)
				mesh->SetMaterial(newMaterial);
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete Material"))
		{
			IMaterial* defaultMaterial = resourceManager->Get<IMaterial>("default.mat");
			mesh->SetMaterial(defaultMaterial);
			if (currentMaterial && currentMaterial != defaultMaterial)
				resourceManager->DeleteMaterial(currentMaterial);
		}

		if (ImGui::Button("Save Material"))
		{
			if (currentMaterial)
				resourceManager->SaveMaterial(currentMaterial);
		}

		ImGui::SameLine();
		if (ImGui::Button("Rename Material") && currentMaterial)
		{
			ImGui::OpenPopup("RenameMaterialPopup");
		}

		if (ImGui::BeginPopup("RenameMaterialPopup"))
		{
			static char newName[128] = "";

			ImGui::InputText("New Name", newName, IM_ARRAYSIZE(newName));

			if (ImGui::Button("OK"))
			{
				std::string oldPath = currentMaterial->GetPath(); 
				std::string directory = oldPath.substr(0, oldPath.find_last_of("/\\") + 1);
				std::string newPath = directory + newName + ".mat";

				IMaterial* defaultMaterial = resourceManager->Get<IMaterial>("default.mat");
				mesh->SetMaterial(defaultMaterial);
				if (currentMaterial && currentMaterial != defaultMaterial)
				{
					if (FileUtils::RenameFile(oldPath, newPath))
					{
						currentMaterial->SetPath(newPath);
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::MeshFilter()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("Mesh Filter"))
	{
		IModel* currentModel = currentObject->GetComponent<IMesh>()->GetModel();

		std::vector<IModel*> allModels = resourceManager->GetAllResources<IModel>();

		IModel* selectedModel = ImGuiHelper::ResourceSelector("Select a Model", allModels, currentModel);

		if (selectedModel)
			currentObject->GetComponent<IMesh>()->SetModel(selectedModel);
	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::MaterialComponent()
{
	IMesh* mesh = currentObject->GetComponent<IMesh>();
	IMaterial* currentMaterial = mesh->GetMaterial();
	IMaterial* defaultMaterial = resourceManager->Get<IMaterial>("default.mat");

	std::vector<ITexture*> allTextures = resourceManager->GetAllResources<ITexture>();

	if (currentMaterial)
	{
		bool isDefault = (currentMaterial == defaultMaterial);

		ImGui::Indent(10.f);
		if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (isDefault)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Default material");
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
				ImGui::BeginDisabled(true);
			}

			ImGui::BeginTable("PBRParams", 2, ImGuiTableFlags_SizingStretchProp);

			auto drawTextureParam = [&](const char* label, ITexture* tex, auto setTexFunc, auto setValueFunc, auto getValueFunc) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(label);
				ImGui::TableSetColumnIndex(1);

				std::string id = "##" + std::string(label);
				ITexture* selected = ImGuiHelper::ResourceSelector(id.c_str(), allTextures, tex);
				if (selected)
					setTexFunc(selected);

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
					{
						if (payload->DataSize == sizeof(ITexture*))
						{
							ITexture* droppedTex = *reinterpret_cast<ITexture* const*>(payload->Data);
							if (droppedTex)
								setTexFunc(droppedTex);
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (!tex)
				{
					float value = getValueFunc();
					if (ImGui::SliderFloat(("##slider_" + std::string(label)).c_str(), &value, 0.0f, 1.0f))
						setValueFunc(value);
				}
				else
				{
					ImGui::SameLine();
					if (ImGui::Button(("Clear##" + std::string(label)).c_str()))
						setTexFunc(nullptr);
				}
				};

			drawTextureParam("Albedo", currentMaterial->GetAlbedoMap(),
				[&](ITexture* t) { currentMaterial->SetAlbedoMap(t); },
				[&](float) {},  
				[&]() { return 0.0f; });

			if (!currentMaterial->GetAlbedoMap()) {
				Vector3D albedo = currentMaterial->GetAlbedo();
				float col[3] = { albedo.x, albedo.y, albedo.z };
				if (ImGui::ColorEdit3("Color##albedo", col))
					currentMaterial->SetAlbedo(Vector3D(col[0], col[1], col[2]));
			}

			drawTextureParam("Roughness", currentMaterial->GetRoughnessMap(),
				[&](ITexture* t) { currentMaterial->SetRoughnessMap(t); },
				[&](float v) { currentMaterial->SetRoughness(v); },
				[&]() { return currentMaterial->GetRoughness(); });

			drawTextureParam("Metallic", currentMaterial->GetMetallicMap(),
				[&](ITexture* t) { currentMaterial->SetMetallicMap(t); },
				[&](float v) { currentMaterial->SetMetallic(v); },
				[&]() { return currentMaterial->GetMetallic(); });

			drawTextureParam("AO", currentMaterial->GetAOMap(),
				[&](ITexture* t) { currentMaterial->SetAOMap(t); },
				[&](float v) { currentMaterial->SetAO(v); },
				[&]() { return currentMaterial->GetAO(); });

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); ImGui::Text("Normal");
			ImGui::TableSetColumnIndex(1);
			std::string normalID = "##normal";
			ITexture* selectedNormal = ImGuiHelper::ResourceSelector(normalID.c_str(), allTextures, currentMaterial->GetNormalMap());
			if (selectedNormal)
				currentMaterial->SetNormalMap(selectedNormal);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
				{
					if (payload->DataSize == sizeof(ITexture*))
					{
						ITexture* droppedTex = *reinterpret_cast<ITexture* const*>(payload->Data);
						if (droppedTex)
							currentMaterial->SetNormalMap(droppedTex);
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (currentMaterial->GetNormalMap())
			{
				ImGui::SameLine();
				if (ImGui::Button("Clear##normal"))
					currentMaterial->SetNormalMap(nullptr);
			}

			ImGui::EndTable();

			if (isDefault)
			{
				ImGui::EndDisabled();
				ImGui::PopStyleVar();
			}
		}

		ImGui::Unindent(10.f);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
}

void InspectorUI::RenderModeComponent()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("Render Settings"))
	{
		IMesh* mesh = currentObject->GetComponent<IMesh>();
		if (!mesh)
			return;

		uint32_t flags = static_cast<uint32_t>(mesh->GetRenderMode());

		bool toon = HasFlag(mesh->GetRenderMode(), RenderMode::Toon);
		bool outline = HasFlag(mesh->GetRenderMode(), RenderMode::Outline);

		if (ImGui::Checkbox("Toon Shading", &toon))
		{
			if (toon)
				flags |= static_cast<uint32_t>(RenderMode::Toon);
			else
				flags &= ~static_cast<uint32_t>(RenderMode::Toon);
		}

		if (ImGui::Checkbox("Outline", &outline))
		{
			if (outline)
				flags |= static_cast<uint32_t>(RenderMode::Outline);
			else
				flags &= ~static_cast<uint32_t>(RenderMode::Outline);
		}

		mesh->SetRenderMode(static_cast<RenderMode>(flags));
	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::LightComponent()
{
	const char* lightTypes[] = { "Directional", "Point", "Spot" };

	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("Light"))
	{
		ILight* currentLight = currentObject->GetComponent<ILight>();
		if (!currentLight)
			return;

		int selectedType = static_cast<int>(currentLight->GetType()); 

		if (ImGui::Combo("Light Type", &selectedType, lightTypes, IM_ARRAYSIZE(lightTypes)))
		{
			LightEngine* lightEngine = gameStateManager->currentGraph->GetLightEngine(); 

			lightEngine->DeleteLight(currentLight);
			currentObject->RemoveComponent(currentLight);

			switch (static_cast<LightTypes>(selectedType))
			{
			case LightTypes::DIRECTIONAL:
				currentObject->components.push_back(
					lightEngine->CreateDirectionalLight(Vector3D(0.f, 0.f, 0.f), Vector3D(0.5f, 0.5f, 0.5f)));
				break;
			case LightTypes::POINT:
				currentObject->components.push_back(
					lightEngine->CreatePointLight(Vector3D(0.f, 0.f, 0.f), Vector3D(0.5f, 0.5f, 0.5f)));
				break;
			case LightTypes::SPOT:
				currentObject->components.push_back(
					lightEngine->CreateSpotLight(Vector3D(0.f, 0.f, 0.f), Vector3D(0.f, 0.f, 0.f), 0.f, 0.f, Vector3D(0.5f, 0.5f, 0.5f)));
				break;
			}

			return; 
		}

		switch (currentLight->GetType())
		{
		case LightTypes::DIRECTIONAL:
			DirectionalLightComponent();
			break;
		case LightTypes::POINT:
			PointLightComponent();
			break;
		case LightTypes::SPOT:
			SpotLightComponent();
			break;
		}
	}
}

void InspectorUI::SpotLightComponent()
{
	SpotLight* light = currentObject->GetComponent<SpotLight>();
	light->position = currentObject->transform.pos;
	ImGui::InputFloat3("Direction", &light->direction.x);
	ImGui::ColorEdit3("Color", &light->color.x);
	ImGui::InputFloat("Spot Angle", &light->spotAngle);
	ImGui::InputFloat("Spot Smooth Angle", &light->spotSmoothAngle);
	ImGui::InputFloat("Intensity", &light->intensity);

	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::DirectionalLightComponent()
{
	DirectionalLight* light = currentObject->GetComponent<DirectionalLight>();
	ImGui::InputFloat3("Direction", &light->direction.x);
	ImGui::ColorEdit3("Color", &light->color.x);
	ImGui::InputFloat("Intensity", &light->intensity);

	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f)); 
}

void InspectorUI::PointLightComponent()
{
	PointLight* light = currentObject->GetComponent<PointLight>();
	light->position = currentObject->transform.pos;
	ImGui::ColorEdit3("Color", &light->color.x);
	ImGui::InputFloat("Intensity", &light->intensity);

	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::RigidBodyComponent()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("RigidBody"))
	{
		if (RigidBody* body = currentObject->GetComponent<RigidBody>())
		{
			static const char* motionTypes[] = { "Static", "Kinematic", "Dynamic" };
			int currentMotionType = static_cast<int>(body->GetMotionType());

			if (ImGui::Combo("Motion Type", &currentMotionType, motionTypes, IM_ARRAYSIZE(motionTypes)))
			{
				JPH::EMotionType motionType = static_cast<JPH::EMotionType>(currentMotionType);
				body->SetMotionType(motionType, JPH::EActivation::Activate);
			}

			bool hasCollisionCallbacksEnabled = body->HasCollisionCallbacksEnabled();
			if (ImGui::Checkbox("CollisionCallBack", &hasCollisionCallbacksEnabled))
			{
				body->SetEnableCollisionCallback(hasCollisionCallbacksEnabled);
			}
		}
	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}


void InspectorUI::SphereColliderComponent()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("SphereCollider"))
	{
		if (SphereCollision* collider = currentObject->GetComponent<SphereCollision>())
		{
			RigidBody* body = currentObject->GetComponent<RigidBody>();

			if (ImGui::InputFloat("Radius", &collider->radius))
				body->UpdateShape(collider);

			ImGui::Checkbox("Debug Draw", &collider->debug);
		}
	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::BoxColliderComponent()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("BoxCollider"))
	{
		if (BoxCollision* collider = currentObject->GetComponent<BoxCollision>())
		{
			RigidBody* body = currentObject->GetComponent<RigidBody>();

			if (ImGui::InputFloat("Width", &collider->size.x))
				body->UpdateShape(collider);
			if (ImGui::InputFloat("Height", &collider->size.y))
				body->UpdateShape(collider);
			if (ImGui::InputFloat("Depth", &collider->size.z))
				body->UpdateShape(collider);

			ImGui::Checkbox("Debug Draw", &collider->debug);
		}
	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::CapsuleColliderComponent()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("CapsuleCollider"))
	{
		if (CapsuleCollision* collider = currentObject->GetComponent<CapsuleCollision>())
		{
			RigidBody* body = currentObject->GetComponent<RigidBody>();

			if (ImGui::InputFloat("Radius", &collider->radius))
				body->UpdateShape(collider);
			if (ImGui::InputFloat("Height", &collider->height))
				body->UpdateShape(collider);

			ImGui::Checkbox("Debug Draw", &collider->debug);
		}
	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::CameraComponent()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("Camera"))
	{
		if (Camera* camera = currentObject->GetComponent<Camera>())
			ImGui::InputFloat3("Offset", &camera->offset.x);
	}
}

void InspectorUI::TextComponent()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("Render Text"))
	{
		Text* text = currentObject->GetComponent<Text>();

		if (ImGui::InputFloat("Position X", &text->x))
			text->x = std::clamp(text->x, 0.0f, 1.0f);

		if (ImGui::InputFloat("Position Y", &text->y))
			text->y = std::clamp(text->y, 0.0f, 1.0f);


		ImGui::InputFloat("Font Size", &text->scale);
		ImGui::ColorEdit3("Color", &text->color.x);

		char buffer[30];
		strncpy_s(buffer, sizeof(buffer), text->text.c_str(), _TRUNCATE);
		buffer[sizeof(buffer) - 1] = '\0';
		ImGui::InputText("Text", buffer, 30);
		text->text = buffer;

	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::SoundDisplay()
{
	ImGui::Indent(10.f);
	if (ImGui::CollapsingHeader("Sound Mixer"))
	{
		SoundComponent* sound = currentObject->GetComponent<SoundComponent>();
		if (sound)
		{
			ISound* currentSound = sound->GetSound();
			std::vector<ISound*> allSounds = resourceManager->GetAllResources<ISound>();

			if (ISound* selectedSound = ImGuiHelper::ResourceSelector("Select a Sound", allSounds, currentSound))
				sound->SetSound(selectedSound);

			float volume = sound->GetVolume();
			if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
				sound->SetVolume(volume);

			bool loop = sound->GetLoop();
			if (ImGui::Checkbox("Loop", &loop))
				sound->SetLoop(loop);

			bool is3D = sound->Get3D();
			if (ImGui::Checkbox("3D", &is3D))
				sound->Set3D(is3D);

			if (ImGui::Button("Play")) sound->Play();
			ImGui::SameLine();
			if (ImGui::Button("Pause")) sound->Pause();
			ImGui::SameLine();
			if (ImGui::Button("Stop")) sound->Stop();
		}
	}
	ImGui::Unindent(10.f);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void InspectorUI::ScriptDisplay()
{
	ImGui::Indent(10.f);
	ImGui::CollapsingHeader(currentObject->GetComponent<ScriptComponent>()->GetScriptName().c_str());
}
