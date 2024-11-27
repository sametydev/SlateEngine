#include "pch.h"
#include "MyTestScript.h"

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
}

void MyTestScript::OnShutdown()
{
}
