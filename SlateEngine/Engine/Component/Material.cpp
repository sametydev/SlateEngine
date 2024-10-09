#include "Material.h"
#include <SlateEngine/Engine/Component/RenderableGeometry.h>
void MaterialComponent::OnInternalInit()
{
}
void MaterialComponent::AddTexture(DXTexture* tex)
{
	textures.emplace_back(tex);
	/*
	if (connectedEntity->HasComponent<RenderableGeometry>() && textures[0] != nullptr) {
		connectedEntity->GetComponent<RenderableGeometry>().SetTexture(textures[0]);
	}
	*/
}
void MaterialComponent::BindPipeline()
{
	/*
	for (auto& v : vertexShaders) {
		v->Bind();
		//v->UpdateInputLayout();
	}
	for (auto& p : pixelShaders) {
		p->Bind();
	}
	*/
	for (auto& s : shaders) {
		s->Bind();
	}
	for (auto& t : textures) {
		t->Bind();
	}
	/*
	if (textures[0] != nullptr) {
		textures[0]->Bind();
	}
	*/
}
void MaterialComponent::OnRender(ID3D11DeviceContext* pDeviceContext)
{
}
void MaterialComponent::OnUpdate(float deltaTime)
{
}

void MaterialComponent::OnInit()
{

}

void MaterialComponent::OnShutdown()
{
}

void MaterialComponent::LateRender()
{
	/*
	if (textures[0] != nullptr) {
		textures[0]->UnBind();
	}
	*/
	for (auto& t : textures) {
		t->UnBind();
	}
}
