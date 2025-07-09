#include <SlateEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <SlateEngine/Engine/Game/Game.h>

DXPixelShader::DXPixelShader()
{
}

DXPixelShader::~DXPixelShader()
{
}

void DXPixelShader::Bind(ID3D11DeviceContext* pDeviceContext)
{
	_NULL_CHECK
	pDeviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

void DXPixelShader::Compile(const WCHAR* csoName, const WCHAR* hlslName, LPCSTR entryName)
{
	HR(CreateShaderFromFile(csoName,hlslName,entryName, "ps_5_0", m_blob.ReleaseAndGetAddressOf()));
	HR(Game::Instance->GetDXDevice()->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));
}

void DXPixelShader::SetConstantBuffer(ID3D11DeviceContext* pDeviceContext,uint32_t slot, uint32_t numOfBuffers, ID3D11Buffer* const* buffer)
{
	pDeviceContext->PSSetConstantBuffers(slot, numOfBuffers, buffer);
}
