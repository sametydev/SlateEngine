#pragma once
#include <SlateEngine/Engine/FileSystem/FileSystem.h>

class Asset {
public:
	Asset() = default;

	SMetaData& GetMetaData() { return assetMetaData; };
private:
	SMetaData assetMetaData;
};