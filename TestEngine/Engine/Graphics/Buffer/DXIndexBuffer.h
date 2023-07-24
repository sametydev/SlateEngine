#pragma once
#include <TestEngine/Engine/Graphics/Buffer/DXBuffer.h>

class DXIndexBuffer : public DXBuffer
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

