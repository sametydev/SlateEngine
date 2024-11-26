#pragma once
#include "Component.h"
#include <Windows.h>
#include <tchar.h>
#include <LuaLibrary.h>
#include <LuaBridge/LuaBridge.h>
#include <SlateEngine/Engine/Utils.h>


struct ENGINE_API LuaScript : public Component
{
public:
	LuaScript() = default;

	~LuaScript() {};

	void OnInternalInit() override;

	void OnUpdate(float deltaTime) override;
	void OnRender(ID3D11DeviceContext* pDeviceContext) override;

	void OnInit() override;
	void OnShutdown() override;

	std::string GetScriptPath() { return scriptPath; };
	void SetScriptPath(const char* val) { scriptPath = val; };

private:
	lua_State* m_luaState = nullptr;
	luabridge::LuaRef updateFunc = nullptr;

	std::string scriptPath;
};

