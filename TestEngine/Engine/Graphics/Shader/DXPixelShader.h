#pragma once
#include <TestEngine/Engine/Graphics/Shader/DXShader.h>

class DXPixelShader: public DXShader
{
public:
	DXPixelShader();
	virtual ~DXPixelShader();


	virtual void Bind();

	virtual void Compile(const WCHAR* csoName, const WCHAR* hlslName, LPCSTR entryName);
	virtual void SetConstantBuffer(UINT slot, UINT numOfBuffers, ID3D11Buffer* const* buffer);

private:
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D10Blob> m_blob;
};
