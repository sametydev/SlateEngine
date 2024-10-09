#pragma once
#include <SlateEngine/Engine/FileSystem/FileSystem.h>

class Asset {
public:
	Asset() = default;
	virtual ~Asset() {};

	SMetaData& GetMetaData() { return assetMetaData; };
private:
	SMetaData assetMetaData;
};