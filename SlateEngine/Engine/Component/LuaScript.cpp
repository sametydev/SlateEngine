#include "LuaScript.h"

#include <LuaLibrary.h>
#include <LuaBridge/LuaBridge.h>
#include <SlateEngine/Engine/Input/InputSystem.h>

void LuaScript::OnInternalInit()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L)
        .beginClass<vec2f>("vec2f")
        .addProperty("x", &vec2f::x)
        .addProperty("y", &vec2f::y)
        .endClass()

        .beginClass<InputSystem>("InputSystem")
        .addStaticProperty("pos", &InputSystem::pos, false)
        .endClass();

    //luabridge::push(L, &gameInstance);
    //lua_setglobal(L, "message");


    luaL_dostring(L, "print(message:TestMessage(InputSystem.pos.x))");

    lua_close(L);
}

void LuaScript::OnUpdate(float deltaTime)
{
}

void LuaScript::OnRender()
{
}