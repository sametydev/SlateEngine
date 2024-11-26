#include "ScriptRegistry.h"
ScriptRegistry* ScriptRegistry::Instance = nullptr;

ScriptRegistry::ScriptRegistry()
{
    if (Instance == nullptr)
    {
        Instance = this;
    }
}

void ScriptRegistry::Register(const std::string& name, ScriptFactory factory)
{
    if (Instance == nullptr) {
        Instance = new ScriptRegistry();
    }
    Instance->registry[name] = factory;
}

Script* ScriptRegistry::Create(const std::string& name)
{
    if (registry.find(name) != registry.end()) {
        return registry[name]();
    }
    return nullptr;
}

std::vector<std::string> ScriptRegistry::GetRegisteredScripts() const {
    std::vector<std::string> names;
    for (const auto& pair : this->registry) {
        names.push_back(pair.first);
    }
    return names;
}
