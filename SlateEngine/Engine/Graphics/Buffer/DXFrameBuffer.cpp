#include <SlateEngine/Engine/Graphics/Buffer/DXFrameBuffer.h>


DXFrameBuffer::DXFrameBuffer(ID3D11Device* device, ID3D11DeviceContext* context)
{
	pDevice = device; pContext = context;
}

DXFrameBuffer::~DXFrameBuffer()
{
	for (int i = 0; i < mRenderTargetViews.size(); i++) {
		mRenderTargetViews[i].Reset();;
	}
	mDepthTexture2D.Reset();
	mDepthStencilView.Reset();
}

void DXFrameBuffer::Create(const FrameBufferDesc& desc)
{
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

	std::vector<ComPtr<ID3D11Texture2D>> mTexture2D;  //TEST+
	mTexture2D.resize(desc.nRenderPass);

	for (int i = 0; i < desc.nRenderPass; ++i) {
		HR(pDevice->CreateTexture2D(&td, nullptr, &mTexture2D[i]));
	}

	D3D11_RENDER_TARGET_VIEW_DESC vd{};
	vd.Format = td.Format;
	vd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	vd.Texture2D.MipSlice = 0;

	mRenderTargetViews.resize(desc.nRenderPass);

	for (int i = 0; i < desc.nRenderPass; ++i) {
		HR(pDevice->CreateRenderTargetView(mTexture2D[i].Get(), &vd, &mRenderTargetViews[i]));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srd{};
	srd.Format = td.Format;
	srd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srd.Texture2D.MostDetailedMip = 0;
	srd.Texture2D.MipLevels = 1;

	mRenderPass.resize(desc.nRenderPass);

	for (int i = 0; i < desc.nRenderPass; ++i) {
		std::shared_ptr<DXTexture> renderpass = std::make_shared<DXTexture>();
		renderpass->Width = width;
		renderpass->Height = height;
		HR(pDevice->CreateShaderResourceView(mTexture2D[i].Get(), &srd, renderpass->mSRV.GetAddressOf()));
		mRenderPass[i] = renderpass;
	}

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

		HR(pDevice->CreateTexture2D(&depthDesc, nullptr, mDepthTexture2D.GetAddressOf()));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd{};
		dsvd.Format = depthDesc.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;

		HR(pDevice->CreateDepthStencilView(mDepthTexture2D.Get(), &dsvd, mDepthStencilView.GetAddressOf()));
	}
	for (int i = 0; i < mTexture2D.size(); i++) {
		if (mTexture2D[i]) {
			mTexture2D[i].Reset();
		}
	}
}

void DXFrameBuffer::Clear(float r, float g, float b, float a)
{
	//========== clear ============
	const float color[4] = { r,g,b,a };
	for (auto rtv : mRenderTargetViews)
		pContext->ClearRenderTargetView(rtv.Get(), color);

	if (mDepthStencilView) {
		pContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	}
}

void DXFrameBuffer::BeginFrame()
{
	//========== save ============

	pContext->OMGetRenderTargets(1, mPrevRTV.GetAddressOf(), mPrevDTV.GetAddressOf());

	//========== set ============
	pContext->OMSetRenderTargets(mRenderTargetViews.size(), mRenderTargetViews.data()->GetAddressOf(),
		mDepthStencilView.Get());

	const float color[4] = { 0.f,0.f,0.f,0.f };
	for (auto rtv : mRenderTargetViews)
		pContext->ClearRenderTargetView(rtv.Get(), color);

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
	for (int i = 0; i < mRenderPass.size(); ++i) {
		mRenderPass[i]->Bind(i);
	}
}

void DXFrameBuffer::UnBindRenderPass()
{
	for (int i = 0; i < mRenderPass.size(); ++i) {
		mRenderPass[i]->UnBind();
	}
}