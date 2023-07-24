#pragma once
#include <TestEngine/Engine/Graphics/Buffer/DXBuffer.h>

class DXConstantBuffer : public DXBuffer
{
public:
	DXConstantBuffer();
	~DXConstantBuffer();

	void Reset() override;
	void BindPS(UINT slot = 0);
	void BindVS(UINT slot = 0);
	void BindPipeline(UINT offset) override;
	void Create(const ConstantBufferDesc& desc);

	void Map(UINT cbSize,const void* dstData);
	void UnMap();
private:
	ComPtr<ID3D11Buffer> m_buffer;
};

