#pragma once

#include <SlateEngine/Engine/Component/Script.h>
#include <unordered_map>
#include <string>

class ScriptRegistry {
public:
    static ScriptRegistry& Instance() {
        static ScriptRegistry instance;
        return instance;
    }

    void Register(const std::string& name, ScriptFactory factory) {
        registry[name] = factory;
    }

    Script* Create(const std::string& name) {
        if (registry.find(name) != registry.end()) {
            return registry[name]();
        }
        return nullptr;
    }

    std::vector<std::string> GetRegisteredScripts() const {
        std::vector<std::string> names;
        for (const auto& pair : registry) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    ScriptRegistry() = default;
    std::unordered_map<std::string, ScriptFactory> registry;
};