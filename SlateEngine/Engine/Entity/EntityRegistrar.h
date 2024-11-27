#pragma once
#include <entt.hpp>
#include <SlateEngine/Engine/Utils.h>

extern ENGINE_API entt::registry globalRegistry;

class ENGINE_API EntityRegistrar {
public:
    static entt::registry& GetRegistry() {
        return globalRegistry;
    }
};