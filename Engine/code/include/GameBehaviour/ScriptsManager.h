#pragma once
#include <vector>
#include <windows.h>
#include <iostream>
#include "Utils/Engine.h"

class GameObject;

class ScriptManager
{
public :
	ScriptManager() = default; 
	~ScriptManager() = default; 

	std::vector<std::string> scriptNames;

	ENGINE_API void LoadGame();
	ENGINE_API void ReloadGame();
	void FreeGame();
	void RecompileGame();

	void Bind(std::vector<GameObject*> objects);
	void Destroy(std::vector<GameObject*> objects);
	ENGINE_API void Start(std::vector<GameObject*> objects);
	void Update(std::vector<GameObject*> objects);

	void AddScriptToVcxproj(const std::string& className);
	void OpenScriptInVisualStudio(const std::string& path, const std::string& filePath);
	void UpdateGameRegistryFile(const std::vector<std::string>& scriptNames);
	void RemoveScriptFromVcxproj(const std::string& className);
	std::string GetScriptNameFromFile(const std::string& file);
	std::string ExecuteAndCaptureOutput(const std::string& command);
	std::string compileLog;

private :
	HMODULE dll;
	std::vector<GameObject*> objs;
};