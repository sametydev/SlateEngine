#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderInformation.h>
#include <unordered_map>
class DXShader
{
public:
	DXShader(){}
	virtual ~DXShader(){}

	virtual void Bind() = 0;

	virtual void Compile(const WCHAR* csoName, const WCHAR* hlslName, LPCSTR entryName) = 0;
	virtual void SetConstantBuffer(UINT slot, UINT numOfBuffers, ID3D11Buffer* const* buffer) = 0;
};