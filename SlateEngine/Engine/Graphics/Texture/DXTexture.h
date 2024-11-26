#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Core/Asset.h>

enum TextureLoaderType {
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

	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return mSRV; }

	UINT Width;
	UINT Height;

private:
	UINT m_slot;
	ComPtr<ID3D11ShaderResourceView> mSRV;
};
