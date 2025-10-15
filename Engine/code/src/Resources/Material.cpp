#include "Resources/Material.h"

#include "Utils/JsonUtils.h"
#include "Utils/FileUtils.h"

#include "Core/Engine.h"
#include "Resources/ResourceManager.h"

MaterialGL::MaterialGL()
{
    resourceManager = Engine::GetInstance()->GetResourceManager(); 
}

void MaterialGL::LoadResource(const char* path)
{
	_path = path;

    nlohmann::json jsonData;
    if (JsonUtils::LoadJsonFile(path, jsonData))
    {
        // --- Albedo ---
        if (jsonData.contains("albedo") && !jsonData["albedo"].is_null())
        {
            if (jsonData["albedo"].is_string())
            {
                std::string texturePath = jsonData["albedo"];
                materialParams.albedoMap = resourceManager->Get<ITexture>( FileUtils::FileName(texturePath).c_str());
            }
            else if (jsonData["albedo"].is_array())
            {
                materialParams.albedo = JsonUtils::ReadVector3D(jsonData["albedo"]);
            }
        }

        // --- Roughness ---
        if (jsonData.contains("roughness") && !jsonData["roughness"].is_null())
        {
            if (jsonData["roughness"].is_string())
            {
                std::string texturePath = jsonData["roughness"];
                materialParams.roughnessMap = resourceManager->Get<ITexture>(FileUtils::FileName(texturePath).c_str());
            }
            else if (jsonData["roughness"].is_number())
            {
                materialParams.roughness = jsonData["roughness"];
            }
        }

        // --- Metallic ---
        if (jsonData.contains("metallic") && !jsonData["metallic"].is_null())
        {
            if (jsonData["metallic"].is_string())
            {
                std::string texturePath = jsonData["metallic"];
                materialParams.metallicMap = resourceManager->Get<ITexture>(FileUtils::FileName(texturePath).c_str());
            }
            else if (jsonData["metallic"].is_number())
            {
                materialParams.metallic = jsonData["metallic"];
            }
        }

        // --- AO ---
        if (jsonData.contains("ao") && !jsonData["ao"].is_null())
        {
            if (jsonData["ao"].is_string())
            {
                std::string texturePath = jsonData["ao"];
                materialParams.aoMap = resourceManager->Get<ITexture>(FileUtils::FileName(texturePath).c_str());
            }
            else if (jsonData["ao"].is_number())
            {
                materialParams.ao = jsonData["ao"];
            }
        }

        // --- Normal ---
        if (jsonData.contains("normal") && !jsonData["normal"].is_null())
        {
            std::string texturePath = jsonData["normal"];
            materialParams.normalMap = resourceManager->Get<ITexture>(FileUtils::FileName(texturePath).c_str());
        }
    }
	else
	{
		std::cerr << "[Erreur] Impossible de charger le fichier material : " << path << std::endl;
		return;
	}
}