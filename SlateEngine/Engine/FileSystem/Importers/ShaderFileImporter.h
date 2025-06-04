#pragma once
#include <SlateEngine\Engine\FileSystem\IAssetImporter.h>

class ShaderFileImporter : public IAssetImporter
{
public:
	ShaderFileImporter();
	~ShaderFileImporter();

	virtual void ImportAsset(FileSystem* fs, std::filesystem::path _p) override;
};

