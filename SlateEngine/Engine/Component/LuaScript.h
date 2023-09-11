#pragma once
#include "Component.h"
#include <Windows.h>
#include <tchar.h>


class TestMessageInstancer {
public:
	TestMessageInstancer();
	~TestMessageInstancer();

	inline void TestMessage() {
		refC++;
		TCHAR buf[100];
		swprintf_s(buf, _T("%d"), refC);
		MessageBox(0, buf, 0, 0);
	}

	int refC = 0;
	static TestMessageInstancer* Instance;
};

struct LuaScript : public Component
{
public:
	LuaScript() = default;

	~LuaScript() {};

	void OnInternalInit() override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
};

