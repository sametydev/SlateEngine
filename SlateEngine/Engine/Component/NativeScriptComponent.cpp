#include "NativeScriptComponent.h"

NativeScriptComponent::NativeScriptComponent()
{
}

void NativeScriptComponent::OnInternalInit()
{
}

void NativeScriptComponent::OnInit()
{
	for (auto& script : scripts) {
		script.second->OnInit();
	}
}

void NativeScriptComponent::OnUpdate(float deltaTime)
{
	for (auto& script : scripts) {
		script.second->OnUpdate(deltaTime);
	}
}

void NativeScriptComponent::OnRender(ID3D11DeviceContext* pDeviceContext)
{
	for (auto& script : scripts) {
		script.second->OnRender(pDeviceContext);
	}
}

void NativeScriptComponent::OnShutdown()
{
	for (auto& script : scripts) {
		script.second->OnShutdown();
	}
}
