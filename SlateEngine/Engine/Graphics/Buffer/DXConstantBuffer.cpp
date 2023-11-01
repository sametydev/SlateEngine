#include <SlateEngine/Engine/Graphics/Buffer/DXConstantBuffer.h>
#include <SlateEngine/Engine/Game/Game.h>

DXConstantBuffer::DXConstantBuffer()
{
}

DXConstantBuffer::~DXConstantBuffer()
{
}

void DXConstantBuffer::Reset()
{
    m_buffer.Reset();
}

void DXConstantBuffer::BindPS(UINT slot)
{
    Game::Instance->GetDXContext()->PSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());

}

void DXConstantBuffer::BindVS(UINT slot)
{
    Game::Instance->GetDXContext()->VSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
}

void DXConstantBuffer::BindPipeline(UINT offset)
{
    BindVS(0);
    BindPS(0);
}

void DXConstantBuffer::Create(const ConstantBufferDesc& desc)
{
    D3D11_BUFFER_DESC cbd{};
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = desc.cbSize;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA sbd{};
    sbd.pSysMem = desc.pData;

    HR(Game::Instance->GetDXDevice()->CreateBuffer(&cbd, nullptr, m_buffer.GetAddressOf()));
}


void DXConstantBuffer::Map(UINT cbSize,const void* dstData)
{
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(Game::Instance->GetDXContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, cbSize, dstData, cbSize);
}

void DXConstantBuffer::UnMap()
{
    Game::Instance->GetDXContext()->Unmap(m_buffer.Get(), 0);
}

void DXConstantBuffer::MapAndUnMap(unsigned int cbSize, const void* dstData)
{
    Map(cbSize, dstData);
    UnMap();
}
