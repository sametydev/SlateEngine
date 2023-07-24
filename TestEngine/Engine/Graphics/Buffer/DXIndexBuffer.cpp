#include <TestEngine/Engine/Graphics/Buffer/DXIndexBuffer.h>
#include <TestEngine/Engine/Game/Game.h>

DXIndexBuffer::DXIndexBuffer()
{
}

DXIndexBuffer::~DXIndexBuffer()
{
}

void DXIndexBuffer::Reset()
{
    m_buffer.Reset();
}

void DXIndexBuffer::Create(const IndexBufferDesc& desc)
{
    
    D3D11_BUFFER_DESC ibd{};
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = desc.cbSize;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = desc.pData;
    HR(Game::Instance->GetDXDevice()->CreateBuffer(&ibd, &sd, m_buffer.GetAddressOf()));
}

void DXIndexBuffer::BindPipeline(UINT offset)
{
    Game::Instance->GetDXContext()->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, offset);
}
