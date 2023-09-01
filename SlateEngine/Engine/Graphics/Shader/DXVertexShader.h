#pragma once
#include <SlateEngine/Engine/Graphics/Shader/DXShader.h>

class DXVertexShader : public DXShader {
public:
	DXVertexShader();
	virtual ~DXVertexShader();


	void Bind() override;

	void Compile(const WCHAR* csoName, const WCHAR* hlslName, LPCSTR entryName) override;

	void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* elements, UINT nElements);
	void UpdateInputLayout();
	void SetConstantBuffer(UINT slot, UINT numOfBuffers, ID3D11Buffer* const* buffer);
private:
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D10Blob> m_blob;
};

