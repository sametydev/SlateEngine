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

class ShaderCache
{
public:
	//External
	[[nodiscard]] static DXShader* CreateShader(ShaderInformation info);
	static std::unordered_map<const char*, std::shared_ptr<DXShader>> externalShader;
};