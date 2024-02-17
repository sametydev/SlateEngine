#pragma once
#include <SlateEngine/Engine/FileSystem/FileSystem.h>

struct DemoAsset{};

class AssetStreamer
{
	//There's must only f-one AssetServer! Its very critical for AssetStreaming
public:
	AssetStreamer();
	~AssetStreamer();


	DemoAsset* RequestAsset(const char* assetPath);
	
	void ResetPool();


	static AssetStreamer* Instance;

private:
	friend class FileSystem;
	bool bDynamicAssetLoading = true;
};

