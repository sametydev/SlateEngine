#include "LuaScript.h"

#include <LuaLibrary.h>
#include <LuaBridge/LuaBridge.h>

void LuaScript::OnInternalInit()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    new TestMessageInstancer();
    TestMessageInstancer* gameInstance = TestMessageInstancer::Instance;

    luabridge::getGlobalNamespace(L)
        .beginClass<TestMessageInstancer>("MessageInstance")
        .addFunction("TestMessage", &TestMessageInstancer::TestMessage)
        .endClass();

    luabridge::push(L, &gameInstance);
    lua_setglobal(L, "message");
    luaL_dostring(L, "print(message:TestMessage())");

    lua_close(L);
}

void LuaScript::OnUpdate(float deltaTime)
{
}

void LuaScript::OnRender()
{
}

TestMessageInstancer* TestMessageInstancer::Instance = nullptr;

TestMessageInstancer::TestMessageInstancer()
{
    if (Instance == nullptr) {
        Instance = this;
    }
}

TestMessageInstancer::~TestMessageInstancer()
{
}
