#pragma once
#include <SlateEngine/Engine/Graphics/Shader/DXShader.h>
#include <SlateEngine/Engine/Graphics/Shader/DXVertexShader.h>
#include <SlateEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderInformation.h>

class ShaderCache
{
public:
	static std::unordered_map<const char*, std::shared_ptr<DXVertexShader>> vertexShaders;
	static std::unordered_map<const char*, std::shared_ptr<DXPixelShader>> pixelShaders;


	[[nodiscard]] inline static DXVertexShader* CreateVertexShader(ShaderInformation info) {
		std::shared_ptr<DXVertexShader> shader = nullptr;

		auto found = vertexShaders.find(info.hlslFile);
		if (found != vertexShaders.end()) {
			return found->second.get();
		}

		shader = std::make_shared<DXVertexShader>();
		shader->Compile(charToWChar(info.csoName), charToWChar(info.hlslFile), info.entryPoint);
		vertexShaders.insert(std::make_pair(info.hlslFile, shader));

		return shader.get();
	}

	[[nodiscard]] inline static DXPixelShader* CreatePixelShader(ShaderInformation info) {
		std::shared_ptr<DXPixelShader> shader = nullptr;

		auto found = pixelShaders.find(info.hlslFile);
		if (found != pixelShaders.end()) {
			return found->second.get();
		}

		shader = std::make_shared<DXPixelShader>();
		shader->Compile(charToWChar(info.csoName), charToWChar(info.hlslFile), info.entryPoint);
		pixelShaders.insert(std::make_pair(info.hlslFile, shader));

		return shader.get();
	}
};

std::unordered_map<const char*, std::shared_ptr<DXVertexShader>> ShaderCache::vertexShaders;
std::unordered_map<const char*, std::shared_ptr<DXPixelShader>> ShaderCache::pixelShaders;

