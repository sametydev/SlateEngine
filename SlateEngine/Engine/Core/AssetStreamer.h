#pragma once
#include <SlateEngine/Engine/FileSystem/FileSystem.h>
#include <SlateEngine/Engine/Core/Asset.h>

class ENGINE_API AssetStreamer
{
	//There's must only f-one AssetServer! Its very critical for AssetStreaming
public:
	AssetStreamer();
	~AssetStreamer();


	void AddAssetToPool(std::string uuid, Asset* asset) {
		assetPool[uuid] = asset;
	};

	Asset* RequestAsset(const char* assetPath);
	Asset* RequestAssetByUUID(const char* uuid);

	template <typename T>
	T* RequestAssetByUUID(const char* uuid) {
		auto it = assetPool.find(uuid);
		Asset* asset = it != assetPool.end() ? it->second : nullptr;

		if (asset == nullptr) {
			return nullptr;
		}

		T* typedAsset = dynamic_cast<T*>(asset);
		return typedAsset;
	}

	void ResetPool();


	static AssetStreamer* Instance;

private:
	friend class FileSystem;
	friend class Asset;
	bool bDynamicAssetLoading = true;

	std::unordered_map<std::string, Asset*> assetPool;

};

