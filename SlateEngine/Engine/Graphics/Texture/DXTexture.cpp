#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <DirectXTex.h>
#include <DDSTextureLoader11.h>
#include <WICTextureLoader11.h>
DXTexture::DXTexture()
{
}

DXTexture::~DXTexture()
{
}

void DXTexture::Load(const wchar_t* filename)
{
	CreateDDSTextureFromFile(DXApplication::Instance->GetDXDevice().Get(), filename, nullptr, mSRV.GetAddressOf());
}

void DXTexture::Bind(UINT slot)
{
	this->slot = slot;
	DXApplication::Instance->GetDXContext().Get()->PSSetShaderResources(slot, 1, mSRV.GetAddressOf());
}

void DXTexture::UnBind()
{
	ID3D11ShaderResourceView* clearView = nullptr;
	DXApplication::Instance->GetDXContext().Get()->PSSetShaderResources(slot, 1, &clearView);
}
