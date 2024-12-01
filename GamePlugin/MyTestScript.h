#pragma once
#include "pch.h"
#include <SlateEngine/Engine/Game/Game.h>
class MyTestScript : public Script
{
	void OnInternalInit() override;

	void OnUpdate(float deltaTime) override;
	void OnRender(ID3D11DeviceContext* pDeviceContext) override;
	void OnInit() override;
	void OnShutdown() override;

private:
	Transform* myTransform = nullptr;
};