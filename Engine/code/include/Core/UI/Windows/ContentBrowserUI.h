#pragma once

#include <string>
#include <vector>
#include <stack>
#include <fstream>

class GameStateManager;
class ResourceManager;
class ScriptManager; 
class InputManager;
class HierarchyUI; 

enum class FileCreationType
{
    Scene,
    Script
};

class ContentBrowserUI
{
public:
    ContentBrowserUI(HierarchyUI* hierarchy);
    ~ContentBrowserUI() = default;

    void SetRootPath(const std::string& rootPath);
    void Render();

    static void DropFile(const std::string& path);
    void DropAsset();

private:
    void DisplayResources(const std::string& path);
	void DisplayDirectories(const std::vector<std::string>& directories, const std::string& currentPath);
    void DisplayFiles(const std::vector<std::string>& files) const; 
    void ContentResources(const std::string& file) const;

    void ContextContentBrowser();
    void OpenFileCreationPopup(FileCreationType type); 

    void ImportResources(std::string currentPath); 

	void DeleteScript(const std::string& className) const;

    std::string currentPath;
    std::string selectedFile;
    std::stack<std::string> pathStack;

    float thumbnailSize = 128.0f; 
    float padding = 16.0f;
    float cellSize = thumbnailSize + padding;

    bool openFilePopup = false;
    FileCreationType currentCreationType;
    char newFileName[128] = "NewFile";

    static inline std::vector<std::string> m_DroppedFiles;

    ResourceManager* resourceManager = nullptr; 
    ScriptManager* scriptManager = nullptr;
    GameStateManager* stateManager = nullptr;
    HierarchyUI* hierarchy = nullptr; 
};
