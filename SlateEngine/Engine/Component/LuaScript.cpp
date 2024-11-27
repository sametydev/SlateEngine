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

    luabridge::push(m_luaState, GetEntity());
    lua_setglobal(m_luaState, "connectedEntity");

    luabridge::push(m_luaState, DXApplication::Instance->GetLogger());
    lua_setglobal(m_luaState, "Log");

    luabridge::push(m_luaState, &InputSystem::pos);
    lua_setglobal(m_luaState, "InputSystem_MousePos");

    updateFunc = luabridge::getGlobal(m_luaState, "OnUpdate");

}

void LuaScript::OnInit()
{
    try
    {
        int status = luaL_loadfile(m_luaState, scriptPath.c_str()) || lua_pcall(m_luaState, 0, 0, 0);
        updateFunc = luabridge::getGlobal(m_luaState, "OnUpdate");

        if (status != LUA_OK) {
            DXApplication::Instance->GetLogger()->AddLog("Error Found!");
            lua_pop(m_luaState, 1); // Remove error message from stack
        }

    }
    catch (const luabridge::LuaException& ex)
    {
        DXApplication::Instance->GetLogger()->AddLog(ex.what());
    }

}

void LuaScript::OnUpdate(float deltaTime)
{
    if (updateFunc) {
        /*
        try { updateFunc(deltaTime); }catch(const luabridge::LuaException& ex){
            DXApplication::Instance->GetLogger()->AddLog(ex.what());
        }*/
        updateFunc(deltaTime);
    }
}

void LuaScript::OnRender(ID3D11DeviceContext* pDeviceContext)
{
}

void LuaScript::OnShutdown()
{
    lua_close(m_luaState);
}
