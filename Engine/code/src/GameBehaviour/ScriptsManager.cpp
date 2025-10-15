#include "GameBehaviour/ScriptsManager.h"
#include "GameBehaviour/Behaviour/MonoBehaviour.h"
#include "Core/DataStructure/GameObject.h"
#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <thread>      
#include <chrono>  
#include "tinyxml2.h"
#include <fstream>
#include <array>

void ScriptManager::ReloadGame()
{
	FreeGame();
	RecompileGame();
	LoadGame();
}

void ScriptManager::LoadGame()
{
	dll = LoadLibrary(L"../Game/x64/Debug/Game.dll");
	if (!dll) {
		std::cerr << "Échec du chargement de Game.dll\n";
		std::exit(0);
	}
	else
		std::cout << "Chargement de la dll REUSSIE" << std::endl;

	using InitFunc = void(*)();
	InitFunc init = reinterpret_cast<InitFunc>(GetProcAddress(dll, "InitScripts"));
	if (init)
	{
		init();
		std::cout << "Fonction trouvée dans la dll\n";
	}
	else
		std::cerr << "Impossible de trouver InitScripts dans la DLL\n";

	auto createScript = reinterpret_cast<MonoBehaviour * (*)(const char*)>(
		GetProcAddress(dll, "CreateScript"));

	if (!createScript)
	{
		std::cerr << "Impossible de trouver CreateScript dans la DLL\n";
		return;
	}

	ScriptComponent::SetCreateScriptFunc(createScript);

	auto destroyScript = reinterpret_cast<void (*)(MonoBehaviour*)>(
		GetProcAddress(dll, "DestroyScript"));

	if (!destroyScript)
	{
		std::cerr << "Impossible de trouver DestroyScript dans la DLL\n";
		return;
	}

	ScriptComponent::SetDestroyScriptFunc(destroyScript);

	scriptNames.clear();

	using GetScriptNamesFunc = const char** (*)(int*);
	int count = 0;
	auto getNames = (GetScriptNamesFunc)GetProcAddress(dll, "GetScriptNames");
	if (getNames)
	{
		const char** names = getNames(&count);
		for (int i = 0; i < count; ++i)
		{
			scriptNames.push_back(names[i]);
		}
	}
}

void ScriptManager::FreeGame()
{
	if (!objs.empty())
		Destroy(objs);

	if (dll)
	{
		FreeLibrary(dll);
		dll = nullptr;
		std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
		std::cout << "Fin de la liberation de la dll" << std::endl;
	}
}

void ScriptManager::RecompileGame()
{
	std::filesystem::remove("../Game/x64/Debug/Game.pdb");

	std::string msbuildCommand =
		"\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe\" " "../Game/Game.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64";

	std::string fullCommand = "cmd /C \"" + msbuildCommand + "\"";

	compileLog = ExecuteAndCaptureOutput(fullCommand);

	if (compileLog.find("error") != std::string::npos) {
		std::cerr << "Erreur lors de la compilation du projet Game\n";
	}
	else {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

std::string ScriptManager::ExecuteAndCaptureOutput(const std::string& command)
{
	std::string result;
	std::array<char, 512> buffer;
	FILE* pipe = _popen(command.c_str(), "r");
	if (!pipe) return "Échec d'ouverture de la commande.";

	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
		result += buffer.data();
	}

	_pclose(pipe);
	return result;
}

void ScriptManager::Bind(std::vector<GameObject*> objects)
{
	objs = objects;

	for (auto& obj : objects)
	{
		if (auto* component = obj->GetComponent<ScriptComponent>())
		{
			component->Bind(component->GetScriptName());
		}
	}
}

void ScriptManager::Destroy(std::vector<GameObject*> objects)
{
	for (auto& obj : objects)
	{
		if (auto* component = obj->GetComponent<ScriptComponent>())
		{
			component->Destroy();
		}
	}
}

void ScriptManager::Start(std::vector<GameObject*> objects)
{
	for (auto& obj : objects)
	{
		if (obj->isActive)
		{
			if (auto* component = obj->GetComponent<ScriptComponent>())
			{
				component->Start();
			}
		}
	}
}

void ScriptManager::Update(std::vector<GameObject*> objects)
{
	for (auto& obj : objects)
	{
		if (obj->isActive)
		{
			if (auto* component = obj->GetComponent<ScriptComponent>())
			{
				component->Update();
			}
		}
	}
}

void ScriptManager::AddScriptToVcxproj(const std::string& className)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile("../Game/Game.vcxproj") != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "Erreur lors du chargement du .vcxproj" << std::endl;
		return;
	}

	tinyxml2::XMLElement* root = doc.RootElement();
	if (!root)
	{
		std::cerr << "Pas de root dans le .vcxproj" << std::endl;
		return;
	}

	tinyxml2::XMLElement* headerGroup = nullptr;
	tinyxml2::XMLElement* sourceGroup = nullptr;

	for (tinyxml2::XMLElement* itemGroup = root->FirstChildElement("ItemGroup"); itemGroup; itemGroup = itemGroup->NextSiblingElement("ItemGroup"))
	{
		if (itemGroup->FirstChildElement("ClInclude") && !headerGroup)
			headerGroup = itemGroup;

		if (itemGroup->FirstChildElement("ClCompile") && !sourceGroup)
			sourceGroup = itemGroup;
	}

	if (!headerGroup)
	{
		headerGroup = doc.NewElement("ItemGroup");
		root->InsertEndChild(headerGroup);
	}
	if (!sourceGroup)
	{
		sourceGroup = doc.NewElement("ItemGroup");
		root->InsertEndChild(sourceGroup);
	}

	tinyxml2::XMLElement* newHeader = doc.NewElement("ClInclude");
	newHeader->SetAttribute("Include", ("../Editor/Assets/Scripts/" + className + ".h").c_str());
	headerGroup->InsertEndChild(newHeader);

	tinyxml2::XMLElement* newSource = doc.NewElement("ClCompile");
	newSource->SetAttribute("Include", ("../Editor/Assets/Scripts/" + className + ".cpp").c_str());
	sourceGroup->InsertEndChild(newSource);

	if (doc.SaveFile("../Game/Game.vcxproj") != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "Erreur lors de la sauvegarde du .vcxproj" << std::endl;
	}
}

void ScriptManager::UpdateGameRegistryFile(const std::vector<std::string>& scriptNames)
{
	std::ofstream registryFile("../Game/GameRegistry.cpp");
	if (!registryFile.is_open())
	{
		std::cerr << "Erreur lors de l'ouverture de GameRegistry.cpp" << std::endl;
		return;
	}

	registryFile << "#include \"Game.h\"\n";
	registryFile << "#include \"GameBehaviour/ScriptRegistry.h\"\n";
	for (const auto& name : scriptNames)
	{
		registryFile << "#include \"Assets/Scripts/" << name << ".h\"\n";
	}

	registryFile << "\nextern \"C\" GAME void InitScripts()\n{\n";

	registryFile << "}\n";
	registryFile.close();
}

void ScriptManager::OpenScriptInVisualStudio(const std::string& solutionPath, const std::string& filePath)
{
	std::wstring solutionW(solutionPath.begin(), solutionPath.end());
	std::wstring fileW(filePath.begin(), filePath.end());

	std::wstring params = L"/edit \"" + fileW + L"\"";
	ShellExecuteW(NULL, L"open", L"devenv.exe", params.c_str(), NULL, SW_SHOWNORMAL);
}

void ScriptManager::RemoveScriptFromVcxproj(const std::string& className)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile("../Game/Game.vcxproj") != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "Erreur lors du chargement du .vcxproj" << std::endl;
		return;
	}

	tinyxml2::XMLElement* root = doc.RootElement();
	if (!root) return;

	for (tinyxml2::XMLElement* itemGroup = root->FirstChildElement("ItemGroup"); itemGroup; itemGroup = itemGroup->NextSiblingElement("ItemGroup"))
	{
		for (tinyxml2::XMLElement* file = itemGroup->FirstChildElement(); file;)
		{
			tinyxml2::XMLElement* next = file->NextSiblingElement();
			const char* include = file->Attribute("Include");

			if (include)
			{
				std::string includeStr = include;
				std::string headerPath = "../Editor/Assets/Scripts/" + className + ".h";
				std::string sourcePath = "../Editor/Assets/Scripts/" + className + ".cpp";

				if (includeStr == headerPath || includeStr == sourcePath)
				{
					itemGroup->DeleteChild(file);
				}
			}

			file = next;
		}
	}

	if (doc.SaveFile("../Game/Game.vcxproj") != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "Erreur lors de la sauvegarde du .vcxproj après suppression\n";
	}
}

std::string ScriptManager::GetScriptNameFromFile(const std::string& file)
{
	std::string name = file;
	size_t lastdot = name.find_last_of(".");
	if (lastdot != std::string::npos)
		name = name.substr(0, lastdot); 
	return name;
}