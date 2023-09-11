#pragma once
#include "Component.h"
#include <Windows.h>
#include <tchar.h>

struct LuaScript : public Component
{
public:
	LuaScript() = default;

	~LuaScript() {};

	void OnInternalInit() override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
};

