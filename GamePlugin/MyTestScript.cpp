#include "pch.h"
#include "MyTestScript.h"
#include <SlateEngine/Engine/Component/RenderableGeometry.h>

void MyTestScript::OnInternalInit()
{
}

void MyTestScript::OnUpdate(float deltaTime)
{
	myTransform->SetRotation({ 0.0f,myTransform->mRotation.y + 15.f * deltaTime,0.0f });
}

void MyTestScript::OnRender(ID3D11DeviceContext* pDeviceContext)
{
}

void MyTestScript::OnInit()
{
	myTransform = GetEntity()->GetComponentAsPointer<Transform>();

	Game::Instance->GetLogger()->AddLog(GetEntity()->GetComponent<EntityName>().name);
	GetEntity()->GetComponent<Transform>().SetPosition({3.2f,3.f,0.0f});
	GetEntity()->AddComponent<RenderableGeometry>();
}

void MyTestScript::OnShutdown()
{
}
