#include "Material.h"
#include <SlateEngine/Engine/Component/RenderableGeometry.h>
void MaterialComponent::OnInternalInit()
{
	rs = (RasterizerState)0;
}
void MaterialComponent::AddTexture(DXTexture* tex)
{
	textures.emplace_back(tex);
}
void MaterialComponent::BindPipeline()
{
	for (auto& s : shaders) {
		s->Bind();
	}
	for (auto& t : textures) {
		t->Bind();
	}
}
void MaterialComponent::OnRender(ID3D11DeviceContext* pDeviceContext)
{
}
void MaterialComponent::OnDraw(ID3D11DeviceContext* pDeviceContext, UINT indices)
{
	DXRasterizerState::SetRasterizerState(rs, pDeviceContext);
	pDeviceContext->DrawIndexed(indices, 0u, 0u);
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
	for (auto& t : textures) {
		t->UnBind();
	}
}
