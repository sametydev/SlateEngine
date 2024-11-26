#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderInformation.h>
#include <unordered_map>

class ENGINE_API IShader
{
public:
	IShader(){}
	virtual ~IShader(){}

	virtual void Bind() = 0;

	virtual void Compile(const WCHAR* csoName, const WCHAR* hlslName, LPCSTR entryName) = 0;
	virtual void SetConstantBuffer(uint32_t slot, uint32_t numOfBuffers, ID3D11Buffer* const* buffer) = 0;
};