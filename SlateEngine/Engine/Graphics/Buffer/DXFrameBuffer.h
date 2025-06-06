#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/Texture/RenderTTexture.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>

struct ENGINE_API FrameBufferDesc {
	UINT width;
	UINT height;
	bool bDepthStencil;
	UINT nRenderPass;
};

class ENGINE_API DXFrameBuffer
{
public:
	DXFrameBuffer(ID3D11Device* device,ID3D11DeviceContext* context);
	virtual ~DXFrameBuffer();
	void Create(const FrameBufferDesc& desc);

	void BeginFrame();
	void BeginFrameById(UINT id);
	void EndFrame(ID3D11RenderTargetView* newRenderTarget, ID3D11DepthStencilView* newDepthStencil);
	void Clear(float* rgba);

	virtual void BindRenderPass();
	virtual void UnBindRenderPass();

	std::vector<ComPtr<ID3D11RenderTargetView>>	mRenderTargetViews;

	ComPtr<ID3D11Texture2D> mDepthTexture2D = nullptr;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;

	ID3D11RenderTargetView* mPrevRTV = nullptr;
	ID3D11DepthStencilView* mPrevDTV = nullptr;

	UINT width;
	UINT height;


	std::vector<std::shared_ptr<DXTexture>>	mRenderPass;

	bool bDepthStencil;
private:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;

	bool Initialized = false;
};