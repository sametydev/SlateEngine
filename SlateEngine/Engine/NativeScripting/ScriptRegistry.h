#pragma once

#include <SlateEngine/Engine/Component/Script.h>
#include <unordered_map>
#include <string>

class ENGINE_API ScriptRegistry {
public:
    ScriptRegistry();
    static ScriptRegistry* Instance;

    void Register(const std::string& name, ScriptFactory factory);

    Script* Create(const std::string& name);

    std::vector<std::string> GetRegisteredScripts() const;

private:
    std::unordered_map<std::string, ScriptFactory> registry;
};