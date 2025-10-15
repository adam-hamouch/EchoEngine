#pragma once
#include "RenderInterface/RHI.h"
#include "Core/Engine.h"

#include "Texture.h"
#include "Model.h"
#include "Material.h"
#include "Sound.h"
#include "SceneGraph.h"
#include "Shader.h"

#include <map>
#include <string>

class GameStateManager;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

    ENGINE_API void SaveResources(const char* path);
    ENGINE_API void LoadResources();

    //Material
    IMaterial* CreateMaterialFile();
    void SaveMaterial(IMaterial* material);
    void DeleteMaterial(IMaterial* material); 

    template <typename T>
    T* CreateResource(const char* path)
    {
        static_assert(std::is_base_of<IResource, T>::value, "T must inherit from IResource");
    	std::string key = std::filesystem::path(path).filename().string();

        auto it = resources.find(key);
        if (it != resources.end())
        {
            delete it->second;
            resources.erase(it);
        }

        IResource* resource = nullptr;
        if (std::is_same_v<T, ITexture>)
            resource = Engine::GetInstance()->GetRenderInterface()->InstanciateTexture();
        else if (std::is_same_v<T, IModel>)
            resource = Engine::GetInstance()->GetRenderInterface()->InstanciateModel();
        else if (std::is_same_v<T, IMaterial>)
            resource = Engine::GetInstance()->GetRenderInterface()->InstanciateMaterial();
        else if (std::is_same_v<T, IShader>)
            resource = Engine::GetInstance()->GetRenderInterface()->InstanciateShader();
        else if (std::is_same_v<T, ISound>)
            resource = new Sound();
        else if (std::is_same_v<T, ISceneGraph>)
            resource = new SceneGraph(); 

        if (!resource)
        {
            std::cerr << "Error: Unable to create resource !" << std::endl;
            return nullptr;
        }

        resource->LoadResource(path);
        resources[key] = resource;
        return dynamic_cast<T*>(resource);
    }

    template <typename T>
    T* Get(const char* key)
    {
        auto it = resources.find(key);
        return (it != resources.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    template <typename T>
    std::vector<T*> GetAllResources()
    {
        std::vector<T*> result;
        for (const auto& pair : resources)
        {
            if (T* resource = dynamic_cast<T*>(pair.second))
                result.push_back(resource);
        }
        return result;
    }

    template <typename T>
    void Delete(const char* key)
    {
        auto it = resources.find(key);
        if (it != resources.end())
        {
            delete it->second;
            resources.erase(it);
        }
    }

private:
	std::map<std::string, IResource*> resources;

	GameStateManager* gameStateManager = nullptr;

	template <typename T>
    void LoadResourcesFromDirectory(const std::string& directoryPath, const::std::string& targetExtension = "");

    void ImportAssets();
    void ExportAssets(nlohmann::json& jsonData);
};
