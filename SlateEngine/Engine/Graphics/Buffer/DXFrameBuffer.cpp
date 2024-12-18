#include "DXFrameBuffer.h"
using RTTType = std::underlying_type_t<RenderTextureCreateFlags>;

DXFrameBuffer::DXFrameBuffer(ID3D11Device* device, ID3D11DeviceContext* context)
{
	pDevice = device; pContext = context;
	desc.bDepthStencil = true;
	desc.height = 1920;
	desc.width = 1080;
}

DXFrameBuffer::~DXFrameBuffer()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthTexture2D);
	SAFE_RELEASE(mDepthStencilView);
}

/*
void DXFrameBuffer::Create(const FrameBufferDesc& desc)
{
	this->desc = desc;
	width = desc.width;
	height = desc.height;
	bDepthStencil = desc.bDepthStencil;

	D3D11_TEXTURE2D_DESC td{};
	td.Width = width;
	td.Height = height;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     //normaly
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.SampleDesc.Count = 1;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	ID3D11Texture2D* mTexture2D;  //TEST+
	HR(pDevice->CreateTexture2D(&td, nullptr, &mTexture2D));
	

	D3D11_RENDER_TARGET_VIEW_DESC vd{};
	vd.Format = td.Format;
	vd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	vd.Texture2D.MipSlice = 0;

	HR(pDevice->CreateRenderTargetView(mTexture2D, &vd, mRenderTargetViews.GetAddressOf()));
	

	D3D11_SHADER_RESOURCE_VIEW_DESC srd{};
	srd.Format = td.Format;
	srd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srd.Texture2D.MostDetailedMip = 0;
	srd.Texture2D.MipLevels = 1;

	std::shared_ptr<DXTexture> renderpass = std::make_shared<DXTexture>();
	renderpass->Width = width;
	renderpass->Height = height;

	HR(pDevice->CreateTexture2D(&textureDesc, NULL, texture.GetAddressOf()));


	HR(pDevice->CreateShaderResourceView(mTexture2D[i], &srd, renderpass->mSRV.GetAddressOf()));
	mRenderPass[i] = renderpass;
	

	if (bDepthStencil) {
		D3D11_TEXTURE2D_DESC depthDesc{};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		HR(pDevice->CreateTexture2D(&depthDesc, nullptr, &mDepthTexture2D));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd{};
		dsvd.Format = depthDesc.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;

		HR(pDevice->CreateDepthStencilView(mDepthTexture2D, &dsvd, &mDepthStencilView));
	}
	for (int i = 0; i < mTexture2D.size(); i++) {
		SAFE_RELEASE(mTexture2D[i]);
	}
}
*/

void DXFrameBuffer::Resize(float w, float h)
{
	flags = RenderTextureCreateFlags::RenderTarget | RenderTextureCreateFlags::Depth | RenderTextureCreateFlags::Linear;

	width = w;
	height = h;

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

		HR(pDevice->CreateTexture2D(&textureDesc, NULL, mRenderPass.GetAddressOf()));

		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = sampleCount > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		HR(pDevice->CreateRenderTargetView(mRenderPass.Get(), &renderTargetViewDesc, mRenderTargetView.GetAddressOf()));

		shaderResViewDesc.Format = textureDesc.Format;
		shaderResViewDesc.ViewDimension = sampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;

		HR(pDevice->CreateShaderResourceView(mRenderPass.Get(), &shaderResViewDesc, textureSRV.GetAddressOf()));
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

		HR(pDevice->CreateTexture2D(&depthDesc, NULL, mDepthTexture2D.GetAddressOf()));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = 1 > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
		HR(pDevice->CreateDepthStencilView(mDepthTexture2D.Get(), &dsvDesc, mDepthStencilView.GetAddressOf()));

		shaderResViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		shaderResViewDesc.ViewDimension = 1 > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
		HR(pDevice->CreateShaderResourceView(mDepthTexture2D.Get(), &shaderResViewDesc, depthSRV.GetAddressOf()));
	}
}

void DXFrameBuffer::Release()
{
	if (mRenderPass)mRenderPass.Reset();
	if (mDepthStencilView)mDepthStencilView.Reset();
	if (mDepthTexture2D)mDepthTexture2D.Reset();
	if (mRenderTargetView)mRenderTargetView.Reset();
	if (mPrevRTV)mPrevRTV.Reset();
	if (mPrevDTV)mPrevDTV.Reset();
	if (depthSRV)depthSRV.Reset();
	if (textureSRV)textureSRV.Reset();
}

void DXFrameBuffer::Clear(float r, float g, float b, float a)
{
	//========== clear ============
	const float color[4] = { r,g,b,a };
	pContext->ClearRenderTargetView(mRenderTargetView.Get(), color);

	if (mDepthStencilView) {
		pContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	}
}

void DXFrameBuffer::BeginFrame()
{
	//========== save ============

	pContext->OMGetRenderTargets(1, mPrevRTV.GetAddressOf(), mPrevDTV.GetAddressOf());

	//========== set ============
	pContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(),
		mDepthStencilView.Get());

	const float color[4] = { 0.f,0.f,0.f,0.f };
	pContext->ClearRenderTargetView(mRenderTargetView.Get(), color);

	if (mDepthStencilView) {
		pContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	}
}

void DXFrameBuffer::EndFrame()
{
	//set back main render target
	pContext->OMSetRenderTargets(1, mPrevRTV.GetAddressOf(), mPrevDTV.Get());

}

void DXFrameBuffer::BindRenderPass()
{
	pContext->PSSetShaderResources(0, 1, textureSRV.GetAddressOf());
}

void DXFrameBuffer::UnBindRenderPass()
{
	pContext->PSSetShaderResources(0, 1, nullptr);
}