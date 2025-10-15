#include "Resources/ResourceManager.h"
#include "GameBehaviour/GameStateManager.h"

#include "Utils/FileUtils.h"
#include "Utils/JsonUtils.h"

ResourceManager::ResourceManager()
{
    gameStateManager = Engine::GetInstance()->GetGameStateManager(); 
}

ResourceManager::~ResourceManager()
{
	for (auto& resource : resources)
	{
		delete resource.second;
	}
}

template <typename T>
void ResourceManager::LoadResourcesFromDirectory(const std::string& directoryPath, const::std::string& targetExtension)
{
    for (const auto& file : FileUtils::ListFiles(directoryPath, false, targetExtension))
    {
        if (!Get<T>(file.c_str()))
        {
            T* resource = CreateResource<T>(file.c_str());
            if (resource)
                std::cout << "[OK] " << " Load : " << file << std::endl;
            else
                std::cerr << "[Error] Failed to load resources" << file << std::endl;
        }
    }
}

void ResourceManager::SaveResources(const char* path)
{
    nlohmann::json jsonAssetsData;
	ExportAssets(jsonAssetsData);
    JsonUtils::SaveJsonFile(path, jsonAssetsData);

    nlohmann::json jsonConfigData;
    jsonConfigData["currentScene"] = FileUtils::FileName(gameStateManager->GetCurrentScene()->GetPath());
    JsonUtils::SaveJsonFile(path, jsonConfigData); 
}

void ResourceManager::LoadResources()
{
    ImportAssets(); 

    nlohmann::json jsonData;
    JsonUtils::LoadJsonFile("Config/SceneConfig.json", jsonData);

    std::string currentSceneName = jsonData["currentScene"]; 
    SceneGraph* graph = Get<SceneGraph>(currentSceneName.c_str()); 

    gameStateManager->SetCurrentScene(graph);

}

void ResourceManager::ImportAssets()
{
    std::cout << "[ResourceManager] Loading models and textures..." << std::endl;

    LoadResourcesFromDirectory<IShader>("Assets/Shaders", ".json");
    LoadResourcesFromDirectory<ITexture>("Assets/Textures");
    LoadResourcesFromDirectory<ITexture>("Editor_Assets");
    LoadResourcesFromDirectory<IModel>("Assets/Meshes");
    LoadResourcesFromDirectory<IMaterial>("Assets/Material");
    LoadResourcesFromDirectory<ISound>("Assets/Sound");
    LoadResourcesFromDirectory<ISceneGraph>("Assets/Scenes");
}

void ResourceManager::ExportAssets(nlohmann::json& jsonData)
{
    for (const auto& [key, resource] : resources)
    {
        if (ITexture* texture = dynamic_cast<ITexture*>(resource))
        {
            jsonData["Textures"][key] = texture->GetPath();
        }
        else if (IModel* model = dynamic_cast<IModel*>(resource))
        {
            jsonData["Models"][key] = model->GetPath();
        }
        else if (IMaterial* material = dynamic_cast<IMaterial*>(resource))
        {
			jsonData["Materials"][key] = material->GetPath();
        }
        else if (ISound* sound = dynamic_cast<ISound*>(resource))
        {
            jsonData["Sounds"][key] = sound->GetPath();
        }
        else if (IShader* shader = dynamic_cast<IShader*>(resource))
        {
            jsonData["Shader"][key] = shader->GetPath();
        }
    }
}

IMaterial* ResourceManager::CreateMaterialFile()
{
    nlohmann::json jsonData;
	jsonData["albedo"] = { 0.5f, 0.5f, 0.5f };
    jsonData["roughness"] = 0.5f; 
	jsonData["metallic"] = 0.f;
	jsonData["ao"] = 1.f;

    std::string outFilename;
    if (JsonUtils::JsonFile("Assets/Material", "material", ".mat", jsonData, outFilename))
    {
        IMaterial* material = CreateResource<IMaterial>(outFilename.c_str());
        return material; 
    }

    return nullptr; 
}

void ResourceManager::SaveMaterial(IMaterial* material)
{
    nlohmann::json jsonData;

    if (material->GetAlbedoMap())
        jsonData["albedo"] = material->GetAlbedoMap()->GetPath();
    else
        jsonData["albedo"] = { JsonUtils::Vector3DToJson(material->GetAlbedo()) }; 

    if (material->GetRoughnessMap())
        jsonData["roughness"] = material->GetRoughnessMap()->GetPath();
    else
        jsonData["roughness"] = material->GetRoughness();

    if (material->GetMetallicMap())
        jsonData["metallic"] = material->GetMetallicMap()->GetPath();
    else
        jsonData["metallic"] = material->GetMetallic(); 

    if (material->GetAOMap())
        jsonData["ao"] = material->GetAOMap()->GetPath();
    else
        jsonData["ao"] = material->GetAO();

    if (material->GetNormalMap())
        jsonData["normal"] = material->GetNormalMap()->GetPath(); 


    JsonUtils::SaveJsonFile(material->GetPath(), jsonData); 
}

void ResourceManager::DeleteMaterial(IMaterial* material)
{
    if (!material) {
        std::cerr << "Null material pointer passed to DeleteMaterial." << std::endl;
        return;
    }

    FileUtils::DeleteFile(material->GetPath());

    std::string key = FileUtils::FileName(material->GetPath()); 
    Delete<IMaterial>(key.c_str()); 
}
