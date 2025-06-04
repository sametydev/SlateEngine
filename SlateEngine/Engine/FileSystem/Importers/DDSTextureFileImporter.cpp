#include "DDSTextureFileImporter.h"

DDSTextureFileImporter::DDSTextureFileImporter()
{
}

DDSTextureFileImporter::~DDSTextureFileImporter()
{
}

void DDSTextureFileImporter::ImportAsset(FileSystem* fs, std::filesystem::path _p)
{
	fs->ProcessMetaFile(_p);
}
