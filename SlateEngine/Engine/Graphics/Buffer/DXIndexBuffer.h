#pragma once
#include <SlateEngine/Engine/Graphics/Buffer/DXBuffer.h>

class ENGINE_API DXIndexBuffer : public DXBuffer
{
public:
    DXIndexBuffer();
    ~DXIndexBuffer();

    void Reset() override;
    void Create(const IndexBufferDesc& desc);
    void BindPipeline(UINT offset) override;

private:
    ComPtr<ID3D11Buffer> m_buffer;
};

