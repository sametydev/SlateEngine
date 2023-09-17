#pragma once
#include <SlateEngine/Engine/Graphics/Shader/IShader.h>

class DXPixelShader: public IShader
{
public:
	DXPixelShader();
	virtual ~DXPixelShader();


	virtual void Bind();

	virtual void Compile(const WCHAR* csoName, const WCHAR* hlslName, LPCSTR entryName);
	virtual void SetConstantBuffer(uint32_t slot, uint32_t numOfBuffers, ID3D11Buffer* const* buffer);

private:
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D10Blob> m_blob;
};
