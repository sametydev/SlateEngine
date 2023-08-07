#include <SlateEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <SlateEngine/Engine/Game/Game.h>

DXPixelShader::DXPixelShader()
{
}

DXPixelShader::~DXPixelShader()
{
}

void DXPixelShader::Bind()
{
	Game::Instance->GetDXContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

void DXPixelShader::Compile(const WCHAR* csoName, const WCHAR* hlslName, LPCSTR entryName)
{
	HR(CreateShaderFromFile(csoName,hlslName,entryName, "ps_5_0", m_blob.ReleaseAndGetAddressOf()));
	HR(Game::Instance->GetDXDevice()->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));
}

void DXPixelShader::SetConstantBuffer(UINT slot, UINT numOfBuffers, ID3D11Buffer* const* buffer)
{
	Game::Instance->GetDXContext()->PSSetConstantBuffers(slot, numOfBuffers, buffer);
}
