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


extern "C" __declspec(dllexport) Script * CreateScript(const char* name) {
    return ScriptRegistry::Instance->Create(name);
}
