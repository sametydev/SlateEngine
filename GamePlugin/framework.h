#pragma once

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
# define NOMINMAX
#endif

#include <windows.h>
#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/Component/Script.h>
#include <SlateEngine/Engine/NativeScripting/ScriptRegistry.h>

#include <vector>
#include <string>

class Instances {
public:
    static Game* Game;
};


extern "C" __declspec(dllexport) std::vector<std::string> GetScriptList() {
    return ScriptRegistry::Instance().GetRegisteredScripts();
}

extern "C" __declspec(dllexport) Script * CreateScript(const char* name) {
    return ScriptRegistry::Instance().Create(name);
}

extern "C" __declspec(dllexport) void SetGameInstance(Game * instance);

extern "C" __declspec(dllexport) void SetGameInstance(Game * instance) {
    Instances::Game = instance;
}
