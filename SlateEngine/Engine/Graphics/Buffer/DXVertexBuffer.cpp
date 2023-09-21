#include <SlateEngine/Engine/Graphics/Buffer/DXVertexBuffer.h>
#include <SlateEngine/Engine/Game/Game.h>

DXVertexBuffer::DXVertexBuffer() : mStride(0)
{
}

DXVertexBuffer::~DXVertexBuffer()
{
}

void DXVertexBuffer::Reset()
{
    m_buffer.Reset();
}

void DXVertexBuffer::Create(const VertexBufferDesc& desc)
{
    mStride = desc.cbStride;

    D3D11_BUFFER_DESC vbd{};
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = desc.cbSize;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = desc.pData;
    HR(Game::Instance->GetDXDevice()->CreateBuffer(&vbd, &sd, m_buffer.GetAddressOf()));
}

void DXVertexBuffer::BindPipeline(UINT offset)
{

    Game::Instance->GetDXContext()->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), &mStride, &offset);
}
