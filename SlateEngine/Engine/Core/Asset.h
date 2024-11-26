#pragma once
#include <SlateEngine/Engine/FileSystem/FileSystem.h>
#include <SlateEngine/Engine/Utils.h>

class ENGINE_API Asset {
public:
	Asset() = default;
	virtual ~Asset() {};

	SMetaData& GetMetaData() { return assetMetaData; };
private:
	SMetaData assetMetaData;
};