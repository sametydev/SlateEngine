#include "DXShader.h"

DXShader* ShaderCache::CreateShader(ShaderInformation info)
{
	std::shared_ptr<DXShader> shader = nullptr;

	auto found = externalShader.find(info.hlslFile);
	if (found != externalShader.end()) {
		return found->second.get();
	}

	shader = std::make_shared<DXShader>();
	shader->Compile(charToWChar(info.csoName), charToWChar(info.hlslFile), info.entryPoint);
	externalShader.insert(std::make_pair(info.hlslFile, shader));

    return shader.get();
}
