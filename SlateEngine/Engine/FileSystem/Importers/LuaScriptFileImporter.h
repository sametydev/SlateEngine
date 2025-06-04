#pragma once
#include <SlateEngine\Engine\FileSystem\IAssetImporter.h>

class LuaScriptFileImporter : public IAssetImporter
{
public:
	LuaScriptFileImporter();
	~LuaScriptFileImporter();

	virtual void ImportAsset(FileSystem* fs, std::filesystem::path _p) override;
};

