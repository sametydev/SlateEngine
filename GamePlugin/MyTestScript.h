#pragma once
#include "pch.h"
#include <SlateEngine/Engine/Game/Game.h>
class MyTestScript : public Script
{
    void Execute() override {
        Instances::g_GameInstance->GetLogger()->AddLog("Hello from DLL - MyTestScript");
    }
	void OnInternalInit() override;

	void OnUpdate(float deltaTime) override;
	void OnRender(ID3D11DeviceContext* pDeviceContext) override;
	void OnInit() override;
	void OnShutdown() override;
};

namespace {
    const bool registered = []() {
        ScriptRegistry::Instance().Register("MyTestScript", []() -> Script* {
            return new MyTestScript();
            });
        return true;
        }();
}