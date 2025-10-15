#pragma once
#include "Math/Math.h"
#include <nlohmann/json.hpp>

class JsonUtils
{
public:
    //Read
    static Vector3D ReadVector3D(const nlohmann::json& j); 
    static Vector4D ReadVector4D(const nlohmann::json& j); 

    //Write
    static nlohmann::json Vector3DToJson(const Vector3D& vec); 
    static nlohmann::json Vector4DToJson(const Vector4D& vec);

    //Create JsonFile
    static bool JsonFile(const std::string& baseDir,
        const std::string& baseName,
        const std::string& extension,
        const nlohmann::json& data,
        std::string& outFilename);

    //Load&Save JsonFile
    static bool LoadJsonFile(const std::string& filepath, nlohmann::json& outJson);
    static bool SaveJsonFile(const std::string& filepath, const nlohmann::json& data); 
};