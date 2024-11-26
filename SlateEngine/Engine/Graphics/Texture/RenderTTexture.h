#pragma once
#include <SlateEngine/Engine/DXConfig.h>

class ENGINE_API RenderTTexture
{
public:
	ComPtr<ID3D11Texture2D> texture;

	ComPtr<ID3D11ShaderResourceView> textureSRV;
	ComPtr<ID3D11ShaderResourceView> depthSRV;

	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	ComPtr<ID3D11Texture2D> depthStencilBuffer;

	unsigned int sampleCount;
	int width, height;
	RenderTextureCreateFlags flags;
private:
	DXGI_FORMAT format;
public:
	RenderTTexture();
	~RenderTTexture();
	void Resize(int _width, int _height);
	void BindTexture(unsigned int slot);
	void BindDepthTexture(unsigned int slot);
	void Release();
	void SetAsRendererTarget();
	void ClearRenderTarget(const float* bgColor);
	ID3D11ShaderResourceView& GetShaderResourceView() { return *textureSRV.Get(); }
	int GetWidth() { return width; };
	int GetHeight() { return height; };

};

