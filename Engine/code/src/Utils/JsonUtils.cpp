#include "Utils/JsonUtils.h"

#include <fstream>

Vector3D JsonUtils::ReadVector3D(const nlohmann::json& j)
{
	return j.is_array() && j.size() == 3 ? Vector3D(j[0], j[1], j[2]) : Vector3D();
}

Vector4D JsonUtils::ReadVector4D(const nlohmann::json& j)
{
	return j.is_array() && j.size() == 4 ? Vector4D(j[0], j[1], j[2], j[3]) : Vector4D();
}

nlohmann::json JsonUtils::Vector3DToJson(const Vector3D& vec)
{
	return nlohmann::json::array({ vec.x, vec.y, vec.z });
}

nlohmann::json JsonUtils::Vector4DToJson(const Vector4D& vec)
{
	return nlohmann::json::array({ vec.x, vec.y, vec.z, vec.w });
}

bool JsonUtils::JsonFile(const std::string& baseDir, const std::string& baseName, const std::string& extension, const nlohmann::json& data, std::string& outFilename)
{
    int id = 1;
    std::filesystem::create_directories(baseDir);

    do {
        outFilename = baseDir + "/" + baseName + "_" + std::to_string(id) + extension;
        ++id;
    } while (std::filesystem::exists(outFilename));

    std::ofstream file(outFilename);
    if (!file.is_open()) {
        std::cerr << "[Error] Unable to create file: " << outFilename << std::endl;
        return false;
    }

    try {
        file << data.dump(4);
        file.close();
        std::cout << "[Info] File created: " << outFilename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] Writing JSON file: " << e.what() << std::endl;
        return false;
    }
}

bool JsonUtils::LoadJsonFile(const std::string& filepath, nlohmann::json& outJson)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[Error] Unable to open JSON file: " << filepath << std::endl;
        return false;
    }

    try {
        file >> outJson;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] Parsing JSON file: " << e.what() << std::endl;
        return false;
    }
}

bool JsonUtils::SaveJsonFile(const std::string& filepath, const nlohmann::json& data)
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[Error] Unable to open file for writing: " << filepath << std::endl;
        return false;
    }

    try {
        file << data.dump(4);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] Failed to write JSON: " << e.what() << std::endl;
        return false;
    }
}
