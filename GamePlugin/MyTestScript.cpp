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
	Entity* e = connectedEntity;
	if (e->HasComponent<EntityName>()) {
		Game::Instance->GetLogger()->AddLog("True");
	}
	else {
		Game::Instance->GetLogger()->AddLog("False");
	}

	//Instances::Game->GetLogger()->AddLog(connectedEntity->GetComponent<EntityName>().name);
}

void MyTestScript::OnInit()
{
}

void MyTestScript::OnShutdown()
{
}
