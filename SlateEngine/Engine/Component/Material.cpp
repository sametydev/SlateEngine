#include "Material.h"
#include <SlateEngine/Engine/Component/RenderableGeometry.h>

void MaterialComponent::OnInternalInit()
{
	Set("$Ambient", vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	Set("$Diffuse", vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	Set("$Specular", vec4f(0.1f, 0.1f, 0.1f, 5.0f));
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
	BindPipeline();

	DXRasterizerState::SetRasterizerState(rs, pDeviceContext);

	pDeviceContext->IASetPrimitiveTopology(topology);
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
