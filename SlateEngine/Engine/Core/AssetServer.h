#pragma once
#include <SlateEngine/Engine/FileSystem/FileSystem.h>

struct DemoAsset{};

class AssetServer
{
	//There's must only f-one AssetServer! Its very critical for AssetStreaming
public:
	AssetServer();
	~AssetServer();


	DemoAsset* RequestAsset(const char* assetPath);
	
	void ResetPool();


	static AssetServer* Instance;

private:
	friend class FileSystem;
	bool bDynamicAssetLoading = true;
};

