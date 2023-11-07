#include "AssetServer.h"

AssetServer* AssetServer::Instance = nullptr;

AssetServer::AssetServer()
{
	if (Instance == nullptr)
	{
		Instance = this;
	}
}

AssetServer::~AssetServer()
{
}

DemoAsset* AssetServer::RequestAsset(const char* assetPath)
{
	for (auto& it : FileSystem::Instance->metaMap)
	{
		if (it.second.path == assetPath)
		{
			switch (it.second.ftype)
			{
				//TEXTURE
				case FILE_TYPE::TEXTURE_DDS:

					break;
				case FILE_TYPE::TEXTURE_WIC:

					break;

				case FILE_TYPE::MESH:

					break;
				default:
					return nullptr;
					break;
			}
		}
	}
    return nullptr;
}

void AssetServer::ResetPool()
{
}
