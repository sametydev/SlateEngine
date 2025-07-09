#pragma once
#include <SlateEngine/Engine/Graphics/Shader/IShader.h>

class ENGINE_API DXVertexShader : public IShader {
public:
	DXVertexShader();
	virtual ~DXVertexShader();


	void Bind(ID3D11DeviceContext* pDeviceContext) override;

	void Compile(const WCHAR* csoName, const WCHAR* hlslName, LPCSTR entryName) override;

	void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* elements, uint32_t nElements);
	void UpdateInputLayout();
	void SetConstantBuffer(ID3D11DeviceContext* pDeviceContext,uint32_t slot, uint32_t numOfBuffers, ID3D11Buffer* const* buffer);
private:
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D10Blob> m_blob;
};

