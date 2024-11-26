#pragma once
#include <SlateEngine/Engine/Graphics/Shader/IShader.h>
#include <SlateEngine/Engine/Graphics/Shader/DXVertexShader.h>
#include <SlateEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderInformation.h>

class ENGINE_API ShaderCache
{
public:
	static std::unordered_map<std::string, std::shared_ptr<DXVertexShader>> vertexShaders;
	static std::unordered_map<std::string, std::shared_ptr<DXPixelShader>> pixelShaders;


	inline static DXVertexShader* CreateVertexShader(ShaderInformation info) {
		std::shared_ptr<DXVertexShader> shader = nullptr;

		auto found = vertexShaders.find(info.displayName);
		if (found != vertexShaders.end()) {
			return found->second.get();
		}

		shader = std::make_shared<DXVertexShader>();
		shader->Compile(charToWChar(info.csoName), charToWChar(info.hlslFile), info.entryPoint);
		vertexShaders.insert(std::make_pair(info.displayName, shader));

		return shader.get();
	}

	inline static DXPixelShader* CreatePixelShader(ShaderInformation info) {
		std::shared_ptr<DXPixelShader> shader = nullptr;

		auto found = pixelShaders.find(info.displayName);
		if (found != pixelShaders.end()) {
			return found->second.get();
		}

		shader = std::make_shared<DXPixelShader>();
		shader->Compile(charToWChar(info.csoName), charToWChar(info.hlslFile), info.entryPoint);
		pixelShaders.insert(std::make_pair(info.displayName, shader));

		return shader.get();
	}

	[[nodiscard]] inline static DXPixelShader* GetPixelShader(const char* name) {
		auto found = pixelShaders.find(name);
		if (found != pixelShaders.end()) {
			return found->second.get();
		}

		return nullptr;
	}

	[[nodiscard]] inline static DXVertexShader* GetVertexShader(const char* name) {
		auto found = vertexShaders.find(name);
		if (found != vertexShaders.end()) {
			return found->second.get();
		}

		return nullptr;
	}

	[[nodiscard]] inline static IShader* GetShader(const char* name) {
		auto found = vertexShaders.find(name);
		if (found != vertexShaders.end()) {
			return found->second.get();
		}

		auto found2 = pixelShaders.find(name);
		if (found2 != pixelShaders.end()) {
			return found2->second.get();
		}

		return nullptr;
	}
};