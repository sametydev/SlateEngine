#pragma once
#include <stdio.h>
#include <filesystem>
#include <SlateEngine\Engine\FileSystem\FileSystem.h>

class IAssetImporter {
public:
	virtual void ImportAsset(FileSystem* fs,std::filesystem::path _p) = 0;
};