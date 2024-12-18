#include "RenderTTexture.h"
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#define DXInstance DXApplication::Instance

using RTTType = std::underlying_type_t<RenderTextureCreateFlags>;

RenderTTexture::RenderTTexture() : sampleCount(1), format(DXGI_FORMAT_R8G8B8A8_UNORM), width(1280), height(720)
{
	flags = RenderTextureCreateFlags::RenderTarget | RenderTextureCreateFlags::Depth | RenderTextureCreateFlags::Linear;
	Resize(1280, 720);
}

RenderTTexture::~RenderTTexture()
{
	Release();
}

void RenderTTexture::SetAsRendererTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	DXInstance->GetDXContext()->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
}

void RenderTTexture::BeginFrame()
{
	//========== save ============

	DXInstance->GetDXContext()->OMGetRenderTargets(1, mPrevRTV.GetAddressOf(), mPrevDTV.GetAddressOf());

	//========== set ============
	DXInstance->GetDXContext()->OMSetRenderTargets(1, renderTargetView.GetAddressOf(),
		depthStencilView.Get());

	const float color[4] = { 0.f,0.f,0.f,0.f };
	DXInstance->GetDXContext()->ClearRenderTargetView(renderTargetView.Get(), color);

	if (depthStencilView) {
		DXInstance->GetDXContext()->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	}
}

void RenderTTexture::EndFrame()
{
	//set back main render target
	DXInstance->GetDXContext()->OMSetRenderTargets(1, mPrevRTV.GetAddressOf(), mPrevDTV.Get());
}

void RenderTTexture::BindTexture(unsigned int slot)
{
	DXInstance->GetDXContext()->PSSetShaderResources(slot, 1, textureSRV.GetAddressOf());
}

void RenderTTexture::BindDepthTexture(unsigned int slot)
{
	DXInstance->GetDXContext()->PSSetShaderResources(slot, 1, depthSRV.GetAddressOf());
}

void RenderTTexture::ClearRenderTarget(const float* colour)
{
	DXInstance->GetDXContext()->ClearRenderTargetView(renderTargetView.Get(), colour);

	if (static_cast<RTTType>(flags) & static_cast<RTTType>(RenderTextureCreateFlags::Depth))DXInstance->GetDXContext()->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void RenderTTexture::Resize(int _width, int _height)
{
	width = _width;
	height = _height;

	Release();

	bool isLinear = static_cast<RTTType>(flags) & static_cast<RTTType>(RenderTextureCreateFlags::Linear);

	D3D11_FILTER filterMode{};
	filterMode = isLinear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc{};
	shaderResViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResViewDesc.Texture2D.MipLevels = 1;

	D3D11_TEXTURE2D_DESC textureDesc{};

	if (static_cast<RTTType>(flags) & static_cast<RTTType>(RenderTextureCreateFlags::RenderTarget))
	{
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc.SampleDesc.Count = 1; //
		textureDesc.SampleDesc.Quality = 0; //sampleCount
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		if (static_cast<RTTType>(flags) & static_cast<RTTType>(RenderTextureCreateFlags::UAV)) {
			textureDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}
		
		HR(DXInstance->GetDXDevice()->CreateTexture2D(&textureDesc, NULL, texture.GetAddressOf()));

		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = sampleCount > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		HR(DXInstance->GetDXDevice()->CreateRenderTargetView(texture.Get(), &renderTargetViewDesc, renderTargetView.GetAddressOf()));

		shaderResViewDesc.Format = textureDesc.Format;
		shaderResViewDesc.ViewDimension = sampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;

		HR(DXInstance->GetDXDevice()->CreateShaderResourceView(texture.Get(), &shaderResViewDesc, textureSRV.GetAddressOf()));
	}

	if (static_cast<RTTType>(flags) & static_cast<RTTType>(RenderTextureCreateFlags::Depth)) {
		D3D11_TEXTURE2D_DESC depthDesc{};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		HR(DXInstance->GetDXDevice()->CreateTexture2D(&depthDesc, NULL, depthStencilBuffer.GetAddressOf()));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = 1 > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
		HR(DXInstance->GetDXDevice()->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, depthStencilView.GetAddressOf()));

		shaderResViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		shaderResViewDesc.ViewDimension = 1 > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
		HR(DXInstance->GetDXDevice()->CreateShaderResourceView(depthStencilBuffer.Get(), &shaderResViewDesc, depthSRV.GetAddressOf()));
	}
}

void RenderTTexture::Release()
{
	if (textureSRV)textureSRV.Reset();
	if (texture)texture.Reset();
	if (depthSRV)depthSRV.Reset();
	if (renderTargetView)renderTargetView.Reset();
	if (depthStencilView)depthStencilView.Reset();
	if (depthStencilBuffer)depthStencilBuffer.Reset();
	if (mPrevRTV)mPrevRTV.Reset();
	if (mPrevDTV)mPrevDTV.Reset();

}
