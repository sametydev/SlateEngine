#include "LuaScriptFileImporter.h"

LuaScriptFileImporter::LuaScriptFileImporter()
{
}

LuaScriptFileImporter::~LuaScriptFileImporter()
{
}

void LuaScriptFileImporter::ImportAsset(FileSystem* fs, std::filesystem::path _p)
{
	fs->ProcessMetaFile(_p);
}
