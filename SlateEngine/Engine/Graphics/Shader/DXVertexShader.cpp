#include <SlateEngine/Engine/Graphics/Shader/DXVertexShader.h>
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Game/Game.h>

DXVertexShader::DXVertexShader()
{
}

DXVertexShader::~DXVertexShader()
{
}

void DXVertexShader::Bind()
{
	_NULL_CHECK
	Game::Instance->GetDXContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	UpdateInputLayout();
}

void DXVertexShader::Compile(const WCHAR* csoName, const WCHAR* hlslName, LPCSTR entryName)
{
	HR(CreateShaderFromFile(csoName, hlslName, entryName, "vs_5_0", m_blob.ReleaseAndGetAddressOf()));
	HR(Game::Instance->GetDXDevice()->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()));
}

void DXVertexShader::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* elements,uint32_t nElements)
{
	HR(Game::Instance->GetDXDevice()->CreateInputLayout(elements, nElements,
		m_blob->GetBufferPointer(), m_blob->GetBufferSize(), m_inputLayout.GetAddressOf()));

	Game::Instance->GetDXContext()->IASetInputLayout(m_inputLayout.Get());
}

void DXVertexShader::UpdateInputLayout()
{
	Game::Instance->GetDXContext()->IASetInputLayout(m_inputLayout.Get());
}

void DXVertexShader::SetConstantBuffer(uint32_t slot, uint32_t numOfBuffers,ID3D11Buffer* const* buffer)
{
	Game::Instance->GetDXContext()->VSSetConstantBuffers(0, 1, buffer);
}
