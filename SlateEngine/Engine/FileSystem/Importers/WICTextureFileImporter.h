#pragma once
#include <SlateEngine\Engine\FileSystem\IAssetImporter.h>

class WICTextureFileImporter : public IAssetImporter
{
public:
	WICTextureFileImporter();
	~WICTextureFileImporter();

	virtual void ImportAsset(FileSystem* fs, std::filesystem::path _p) override;
};
