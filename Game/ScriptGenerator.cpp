#pragma once
#include "GameBehaviour/ScriptRegistry.h"
#include "GameBehaviour/Behaviour/MonoBehaviour.h"

extern "C" __declspec(dllexport) MonoBehaviour* CreateScript(const char* name)
{
    return ScriptRegistry::Create(name);
}

extern "C" __declspec(dllexport) void DestroyScript(MonoBehaviour* instance)
{
    delete instance;
}

extern "C" __declspec(dllexport) const char** GetScriptNames(int* count)
{
    const auto& registry = ScriptRegistry::GetRegistry();
    static std::vector<const char*> names;

    names.clear();
    for (const auto& [key, _] : registry)
        names.push_back(key.c_str());

    *count = static_cast<int>(names.size());
    return names.data();
}