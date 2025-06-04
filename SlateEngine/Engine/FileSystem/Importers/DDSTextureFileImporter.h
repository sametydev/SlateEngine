#pragma once
#include <SlateEngine\Engine\FileSystem\IAssetImporter.h>


class DDSTextureFileImporter : public IAssetImporter
{
public:
	DDSTextureFileImporter();
	~DDSTextureFileImporter();

	virtual void ImportAsset(FileSystem* fs, std::filesystem::path _p) override;
};
