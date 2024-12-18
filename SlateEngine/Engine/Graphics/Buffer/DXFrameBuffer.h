#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/Texture/RenderTTexture.h>

struct FrameBufferDesc {
	UINT width;
	UINT height;
	bool bDepthStencil;
};

class DXFrameBuffer
{
public:
	DXFrameBuffer(ID3D11Device* device,ID3D11DeviceContext* context);
	virtual ~DXFrameBuffer();
	void Create(const FrameBufferDesc& desc);

	void Resize(float w, float h);
	void Release();

	void BeginFrame();
	void EndFrame();
	void Clear(float r, float g, float b, float a);

	virtual void BindRenderPass();
	virtual void UnBindRenderPass();

	ComPtr<ID3D11RenderTargetView>	mRenderTargetView = nullptr;

	ComPtr<ID3D11Texture2D> mDepthTexture2D = nullptr;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;

	ComPtr<ID3D11RenderTargetView> mPrevRTV = nullptr;
	ComPtr<ID3D11DepthStencilView> mPrevDTV = nullptr;

	UINT width;
	UINT height;

	ComPtr<ID3D11Texture2D> mRenderPass = nullptr;
	ComPtr<ID3D11ShaderResourceView> textureSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> depthSRV = nullptr;

	bool bDepthStencil;
	unsigned int sampleCount;
	RenderTextureCreateFlags flags;
	DXGI_FORMAT format;
private:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	FrameBufferDesc desc{};
};