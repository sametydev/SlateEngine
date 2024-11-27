#include "pch.h"
#include "MyTestScript.h"
#include <SlateEngine/Engine/Component/RenderableGeometry.h>

void MyTestScript::OnInternalInit()
{
}

void MyTestScript::OnUpdate(float deltaTime)
{
}

void MyTestScript::OnRender(ID3D11DeviceContext* pDeviceContext)
{
}

void MyTestScript::OnInit()
{
	Game::Instance->GetLogger()->AddLog(GetEntity()->GetComponent<EntityName>().name);
	GetEntity()->GetComponent<Transform>().SetPosition({3.2f,3.f,0.0f});
	GetEntity()->AddComponent<RenderableGeometry>();
}

void MyTestScript::OnShutdown()
{
}
