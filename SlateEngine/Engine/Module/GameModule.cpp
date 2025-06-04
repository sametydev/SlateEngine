#include "GameModule.h"

constexpr const char* GAME_MODULE_DLL_NAME = "GamePlugin.dll";

GameModule::GameModule()
{
}

GameModule::~GameModule()
{
    FreeLibrary(m_hModule);
}

bool GameModule::Initialize()
{
    m_hModule = LoadLibraryA(GAME_MODULE_DLL_NAME);
    if (!m_hModule) {
        MessageBoxA(0, "Error on GameModule DLL load!", 0, 0);
        throw new std::exception("Error on GameModule DLL load!");
        return false;
    }
    return true;
}
