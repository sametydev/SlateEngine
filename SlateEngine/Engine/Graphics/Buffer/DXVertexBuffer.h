#pragma once
#include <SlateEngine/Engine/Graphics/Buffer/DXBuffer.h>

class ENGINE_API DXVertexBuffer : public DXBuffer
{
public:
    DXVertexBuffer();
    ~DXVertexBuffer();

    void Reset() override;
    void Create(const VertexBufferDesc& desc);
    void BindPipeline(UINT offset) override;

private:
    ComPtr<ID3D11Buffer> m_buffer;
    UINT mStride;
};

