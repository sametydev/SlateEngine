#pragma once
#include <SlateEngine/Engine/Component/Script.h>
#include <SlateEngine/Engine/NativeScripting/ScriptRegistry.h>

class GameModule
{
public:
	GameModule();
	~GameModule();

	bool Initialize();

	HMODULE GetHModuleOfGameModule() { return m_hModule; };
private:
	HMODULE m_hModule;
};

