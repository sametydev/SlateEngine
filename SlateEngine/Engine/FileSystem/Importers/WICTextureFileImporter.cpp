#include "WICTextureFileImporter.h"

WICTextureFileImporter::WICTextureFileImporter()
{
}

WICTextureFileImporter::~WICTextureFileImporter()
{
}

void WICTextureFileImporter::ImportAsset(FileSystem* fs, std::filesystem::path _p)
{
	fs->ProcessMetaFile(_p);
}
