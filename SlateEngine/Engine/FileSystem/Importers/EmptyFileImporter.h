#pragma once
#include <SlateEngine\Engine\FileSystem\IAssetImporter.h>

class EmptyFileImporter : public IAssetImporter
{
public:
	EmptyFileImporter();
	~EmptyFileImporter();

	virtual void ImportAsset(FileSystem* fs, std::filesystem::path _p) override;
};