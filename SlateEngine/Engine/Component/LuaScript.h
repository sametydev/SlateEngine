#pragma once
#include "Component.h"
#include <Windows.h>
#include <tchar.h>
#include <LuaLibrary.h>
#include <LuaBridge/LuaBridge.h>

struct LuaScript : public Component
{
public:
	LuaScript() = default;

	~LuaScript() {};

	void OnInternalInit() override;

	void OnUpdate(float deltaTime) override;
	void OnRender() override;

	void OnInit() override;
	void OnShutdown() override;

	void LoadScript(const char* path);

private:
	lua_State* m_luaState = nullptr;
	luabridge::LuaRef updateFunc = nullptr;

	std::string scriptPath;
};

