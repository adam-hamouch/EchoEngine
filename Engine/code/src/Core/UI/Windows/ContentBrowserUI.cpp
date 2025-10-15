#include "Core/UI/Windows/ContentBrowserUI.h"
#include "Core/Engine.h"

#include "GameBehaviour/ScriptsManager.h"

#include "Resources/ResourceManager.h"

#include "GameBehaviour/GameStateManager.h"

#include <imgui.h>
#include <imgui_internal.h>

#ifdef CreateFile
#undef CreateFile
#endif

#ifdef DeleteFile
#undef DeleteFile
#endif

#include "Core/UI/Windows/HierarchyUI.h"
#include "Utils/FileUtils.h"

ContentBrowserUI::ContentBrowserUI(HierarchyUI* _hierarchy) : hierarchy(_hierarchy)  
{
	SetRootPath("Assets");

	scriptManager = Engine::GetInstance()->GetScriptsManager(); 
	resourceManager = Engine::GetInstance()->GetResourceManager();
	stateManager = Engine::GetInstance()->GetGameStateManager(); 
}

void ContentBrowserUI::SetRootPath(const std::string& rootPath)
{
	currentPath = rootPath;
	while (!pathStack.empty()) pathStack.pop();
}

void ContentBrowserUI::Render()
{
	ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("Back") && !pathStack.empty()) {
		currentPath = pathStack.top();
		pathStack.pop();
	}

	ContextContentBrowser();

	DisplayResources(currentPath);

	ImportResources(currentPath);

	ImGui::End();
}

void ContentBrowserUI::DisplayResources(const std::string& path)
{
	float windowWidth = ImGui::GetWindowSize().x;
	int columnCount = static_cast<int>(windowWidth / cellSize);
	ImGui::Columns(columnCount, nullptr, false);

    std::vector<std::string> directories = FileUtils::ListSubdirectories(path);
	DisplayDirectories(directories, path);

	std::vector<std::string> files = FileUtils::ListFiles(path, true);
	DisplayFiles(files);
}

void ContentBrowserUI::ImportResources(std::string currentPath)
{
	for (const auto& src : m_DroppedFiles)
	{
		std::filesystem::path sourcePath(src);
		std::string filename = sourcePath.filename().string();
		std::string destination = currentPath + "/" + filename;

		try
		{
			std::filesystem::copy(sourcePath, destination, std::filesystem::copy_options::overwrite_existing);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error copy: " << e.what() << std::endl;
		}

		if (filename.ends_with(".png") || filename.ends_with(".jpg"))
		{
			resourceManager->CreateResource<ITexture>(destination.c_str());
		}
		else if (filename.ends_with(".obj"))
		{
			resourceManager->CreateResource<IModel>(destination.c_str());
		}
	}

	m_DroppedFiles.clear();
}

void ContentBrowserUI::DropFile(const std::string& path)
{
	m_DroppedFiles.push_back(path);
}

void ContentBrowserUI::DropAsset()
{
	ImVec2 winPos = ImGui::GetWindowPos();
	ImVec2 winSize = ImGui::GetWindowSize();

	float x = winPos.x + winSize.x;
	float y = winPos.y + winSize.y;
	ImVec2 size(x, y); 

	ImRect dropZone(winPos, size);

	if (ImGui::BeginDragDropTargetCustom(dropZone, ImGui::GetID("CustomDropZone")))
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ITEM");
		if (payload)
		{
			const char* droppedPath = static_cast<const char*>(payload->Data);
			std::string pathStr = droppedPath;

			if (pathStr.ends_with(".obj"))
			{
				IModel* model = resourceManager->Get<IModel>(droppedPath);
				stateManager->GetCurrentScene()->CreateGameObject("New GameObject", model, resourceManager->Get<IMaterial>("default.mat"), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f));
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void ContentBrowserUI::DisplayDirectories(const std::vector<std::string>& directories, const std::string& currentPath)
{
	for (const auto& dir : directories)
	{
		ImGui::BeginGroup();
		ImGui::PushID(dir.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		if (ImGui::ImageButton(dir.c_str(), (ImTextureID)(intptr_t)resourceManager->Get<ITexture>("folder.png")->GetTextureID(), ImVec2(thumbnailSize, thumbnailSize)))
		{
			pathStack.push(currentPath);
			this->currentPath = currentPath + "/" + dir;
		}

		ImGui::TextWrapped(dir.c_str());
		ImGui::PopStyleColor();
		ImGui::PopID();
		ImGui::EndGroup();
		ImGui::NextColumn();
	}
}

void ContentBrowserUI::DisplayFiles(const std::vector<std::string>& files) const
{
	for (const auto& file : files)
	{
		ImGui::BeginGroup();

		ContentResources(file);

		ImGui::EndGroup();
		ImGui::NextColumn();
	}
}

void ContentBrowserUI::ContentResources(const std::string& file) const
{
	ImGui::BeginGroup();

	std::string extension = FileUtils::GetExtension(file);
	std::string filename = FileUtils::FileName(file);

	const char* dragType = "FILE";
	void* payloadData = nullptr;
	int payloadSize = 0;

	ImVec2 previewSize = ImVec2(thumbnailSize, thumbnailSize);
	ITexture* icon = nullptr;

	if (extension == ".png" || extension == ".jpg")
	{
		icon = resourceManager->Get<ITexture>(file.c_str());
		dragType = "TEXTURE";
		payloadData = &icon; 
		payloadSize = sizeof(ITexture*);
	}
	else if (extension == ".obj")
	{
		icon = resourceManager->Get<ITexture>("obj.png");
		dragType = "ITEM";
		payloadData = (void*)file.c_str(); 
		payloadSize = (int)strlen(file.c_str()) + 1;
	}
	else if (extension == ".h")
	{
		icon = resourceManager->Get<ITexture>("h.png");
	}
	else if (extension == ".cpp")
	{
		icon = resourceManager->Get<ITexture>("cpp.png");
	}

	ImGui::PushID(file.c_str()); 

	bool clicked = false;

	if (icon)
	{
		clicked = ImGui::ImageButton(file.c_str(), (ImTextureID)(intptr_t)icon->GetTextureID(), previewSize);

		if (ImGui::BeginDragDropSource())
		{
			void* payloadPtr = icon;
			ImGui::SetDragDropPayload(dragType, payloadData, payloadSize);
			ImGui::Text("Dragging: %s", filename.c_str());
			ImGui::EndDragDropSource();
		}
	}
	else
	{
		clicked = ImGui::Button("", previewSize);
	}

	ImGui::PopID();

	if (clicked)
	{
		if (extension == ".scene")
		{
			hierarchy->ClearCurrentObject(); 
			ISceneGraph* graph = resourceManager->Get<SceneGraph>(filename.c_str());
			stateManager->SetCurrentScene(graph);
		}
		else if (extension == ".h")
		{
			scriptManager->OpenScriptInVisualStudio("../EchoEngine.sln", "Assets/Scripts/" + file);
		}
		else if (extension == ".cpp")
		{
			scriptManager->OpenScriptInVisualStudio("../EchoEngine.sln", "Assets/Scripts/" + file);

		}
	}

	if (ImGui::BeginPopupContextItem(("ContextMenu##" + file).c_str()))
	{
		if (extension == ".scene")
		{
			if (ImGui::MenuItem("Delete"))
			{
				ISceneGraph* graph = resourceManager->Get<SceneGraph>(filename.c_str());

				if (stateManager->GetCurrentScene() != graph)
				{
					FileUtils::DeleteFile(graph->GetPath());
					resourceManager->Delete<SceneGraph>(filename.c_str());
				}
				else
				{
					std::cerr << "[Error] Cannot delete the scene currently in use: " << filename << std::endl;
				}
			}
		}
		else if (extension == ".cpp")
		{
			if (ImGui::MenuItem("Delete"))
			{
				DeleteScript(scriptManager->GetScriptNameFromFile(file));
			}
		}
		else if (extension == ".h")
		{
			if (ImGui::MenuItem("Delete"))
			{
				DeleteScript(scriptManager->GetScriptNameFromFile(file));
			}
		}

		ImGui::EndPopup();
	}

	float textWidth = ImGui::CalcTextSize(filename.c_str()).x;
	float buttonWidth = previewSize.x;
	float cursorX = ImGui::GetCursorPosX();
	float offsetX = (buttonWidth - textWidth) * 0.5f;

	ImGui::SetCursorPosX(cursorX + offsetX);
	ImGui::TextWrapped("%s", filename.c_str());
	ImGui::EndGroup();
}

void ContentBrowserUI::ContextContentBrowser()
{
	if (ImGui::BeginPopupContextWindow("RightClickBrowser"))
	{
		if (ImGui::MenuItem("Create new Scene"))
		{
			OpenFileCreationPopup(FileCreationType::Scene);
		}

		if (ImGui::Button("New Script"))
		{
			OpenFileCreationPopup(FileCreationType::Script);
		}

		ImGui::EndPopup();
	}

	if (openFilePopup)
	{
		ImGui::OpenPopup("Create New File");
		openFilePopup = false;
	}

	if (ImGui::BeginPopupModal("Create New File", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Name", newFileName, IM_ARRAYSIZE(newFileName));

		if (ImGui::Button("Create"))
		{
			std::string name = newFileName;

			if (currentCreationType == FileCreationType::Scene)
			{
				std::string outPath;
				FileUtils::CreateFile(currentPath, name, ".scene", outPath);
				resourceManager->CreateResource<ISceneGraph>(outPath.c_str()); 
			}
			else if (currentCreationType == FileCreationType::Script)
			{
				scriptManager->scriptNames.push_back(name);
				scriptManager->UpdateGameRegistryFile(scriptManager->scriptNames);
				scriptManager->AddScriptToVcxproj(name);

				std::ifstream headerTemplateFile("Templates/MonoBehaviourTemplateH.txt");
				std::ifstream sourceTemplateFile("Templates/MonoBehaviourTemplateCPP.txt");
				std::string headerTemplate((std::istreambuf_iterator<char>(headerTemplateFile)), {});
				std::string sourceTemplate((std::istreambuf_iterator<char>(sourceTemplateFile)), {});

				size_t pos;
				while ((pos = headerTemplate.find("{{ClassName}}")) != std::string::npos)
					headerTemplate.replace(pos, 13, name);
				while ((pos = sourceTemplate.find("{{ClassName}}")) != std::string::npos)
					sourceTemplate.replace(pos, 13, name);

				std::filesystem::create_directories("Assets/Scripts/");
				std::ofstream headerFile("Assets/Scripts/" + name + ".h");
				std::ofstream sourceFile("Assets/Scripts/" + name + ".cpp");

				headerFile << headerTemplate;
				sourceFile << sourceTemplate;
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ContentBrowserUI::OpenFileCreationPopup(FileCreationType type)
{
	currentCreationType = type;
	strcpy_s(newFileName, "NewFile"); 
	openFilePopup = true;
}

void ContentBrowserUI::DeleteScript(const std::string& className) const
{
	std::filesystem::remove("Assets/Scripts/" + className + ".cpp");
	std::filesystem::remove("Assets/Scripts/" + className + ".h");

	scriptManager->scriptNames.erase(std::remove(scriptManager->scriptNames.begin(), scriptManager->scriptNames.end(), className), scriptManager->scriptNames.end());
	scriptManager->UpdateGameRegistryFile(scriptManager->scriptNames);
	scriptManager->RemoveScriptFromVcxproj(className);

	scriptManager->ReloadGame();
}

