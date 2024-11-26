#pragma once
#include <SlateEngine/Engine/Graphics/Buffer/DXBuffer.h>

class ENGINE_API DXConstantBuffer : public DXBuffer
{
public:
	DXConstantBuffer();
	~DXConstantBuffer();

	void Reset() override;
	void BindPS(unsigned int slot = 0);
	void BindVS(unsigned int slot = 0);
	void BindPipeline(unsigned int offset) override;
	void Create(const ConstantBufferDesc& desc);

	void Map(unsigned int cbSize,const void* dstData);
	void UnMap();

	void MapAndUnMap(unsigned int cbSize, const void* dstData);
private:
	ComPtr<ID3D11Buffer> m_buffer;
};

