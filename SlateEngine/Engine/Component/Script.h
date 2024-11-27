#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <SlateEngine/Engine/Component/Component.h>

class ENGINE_API Script : public Component {
public:
    virtual ~Script() = default;
};

using ScriptFactory = std::function<Script* ()>;