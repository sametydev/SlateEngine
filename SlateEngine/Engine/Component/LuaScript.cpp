#include "LuaScript.h"

#include <SlateEngine/Engine/Input/InputSystem.h>
#include <SlateEngine/Engine/Component/Transform.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Core/ILogger.h>

void LuaScript::OnInternalInit()
{

    m_luaState = (luaL_newstate());
    luaL_openlibs(m_luaState);

    luabridge::getGlobalNamespace(m_luaState)
        .beginClass<vec2f>("vec2f")
        .addConstructor<void(*) (void)>()
        .addConstructor<void(*) (float, float)>()
        .addProperty("x", &vec2f::x)
        .addProperty("y", &vec2f::y)
        .endClass()

        .beginClass<vec3f>("vec3f")
        .addConstructor<void(*) (void)>()
        .addConstructor<void(*) (float, float, float)>()
        .addProperty("x", &vec3f::x)
        .addProperty("y", &vec3f::y)
        .addProperty("z", &vec3f::y)
        .endClass()

        .beginClass<Component>("Component")

        .endClass()

        .beginClass<Transform>("Transform")
        .addFunction("SetRotation", &Transform::SetRotation)
        .addFunction("SetSetPosition", &Transform::SetPosition)
        .endClass()

        .beginClass<ILogger>("ILogger")
        .addFunction("Add",&ILogger::AddLog)
        .endClass()


        .beginClass<Entity>("Entity")
        .addFunction("GetComponent_Transform", &Entity::GetComponent<Transform>)
        .addFunction("HasComponent_Transform", &Entity::HasComponent<Transform>)
        .endClass()

        .beginClass<InputSystem>("InputSystem")
        .addStaticProperty("pos", &InputSystem::pos, false)
        .endClass();

    luabridge::push(m_luaState, &connectedEntity);
    lua_setglobal(m_luaState, "connectedEntity");

    luabridge::push(m_luaState, DXApplication::Instance->GetLogger());
    lua_setglobal(m_luaState, "Log");

    updateFunc = luabridge::getGlobal(m_luaState, "OnUpdate");

}

void LuaScript::OnInit()
{
    luaL_dofile(m_luaState, scriptPath.c_str());
    updateFunc = luabridge::getGlobal(m_luaState, "OnUpdate");
}

void LuaScript::OnUpdate(float deltaTime)
{
    if (updateFunc) {
        updateFunc(deltaTime);
    }
}

void LuaScript::OnRender()
{
}

void LuaScript::OnShutdown()
{
    lua_close(m_luaState);
}

void LuaScript::LoadScript(const char* path)
{
    scriptPath  = PathMaker::Make(gDXApp->GetWorkingDir(), path);
}
