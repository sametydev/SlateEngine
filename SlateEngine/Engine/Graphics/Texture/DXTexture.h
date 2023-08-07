#pragma once
#include <SlateEngine/Engine/DXConfig.h>
class DXTexture
{
public:
	DXTexture();
	~DXTexture();

	void Load(const wchar_t* filename);
	void Bind(UINT slot = 0);
	void UnBind();

	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return mSRV; }

private:
	UINT slot;
	ComPtr<ID3D11ShaderResourceView> mSRV;
};
