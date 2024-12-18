#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Core/Asset.h>

enum ENGINE_API TextureLoaderType {
	DDS = 0,
	WIC
};

class ENGINE_API DXTexture : public Asset
{
public:
	DXTexture();
	~DXTexture();

	void Load(const char* filename, TextureLoaderType type);
	void Bind(UINT slot = 0);
	void UnBind();

	inline ID3D11ShaderResourceView* GetShaderResourceView() { return mSRV.Get(); }

	UINT Width;
	UINT Height;
	ComPtr<ID3D11ShaderResourceView> mSRV;

private:
	UINT m_slot;
};
