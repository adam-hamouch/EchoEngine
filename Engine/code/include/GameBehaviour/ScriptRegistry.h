#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

class MonoBehaviour;

class ScriptRegistry
{
public:
    using ScriptFactory = std::function<MonoBehaviour* ()>;

    static void Register(const std::string& name, ScriptFactory factory)
    {
        GetRegistry()[name] = factory;
    }

    static MonoBehaviour* Create(const std::string& name)
    {
        auto it = GetRegistry().find(name);
        if (it != GetRegistry().end())
            return it->second();
        return nullptr;
    }

    static std::unordered_map<std::string, ScriptFactory>& GetRegistry()
    {
        static std::unordered_map<std::string, ScriptFactory> registry;
        return registry;
    }
};

#define REGISTER_SCRIPT(CLASS) \
    namespace { const bool _##CLASS##_registered = []() { \
        ScriptRegistry::Register(#CLASS, []() -> MonoBehaviour* { return new CLASS(); }); \
        return true; \
    }(); }