#pragma once
#include <SlateEngine/Engine/Utils.h>
#include <unordered_map>
#include <SlateEngine\Engine\FileSystem\IAssetImporter.h>
#include <SlateEngine\Engine\FileSystem\Importers\EmptyFileImporter.h>
#include <SlateEngine\Engine\FileSystem\Importers\ShaderFileImporter.h>
#include <SlateEngine\Engine\FileSystem\Importers\LuaScriptFileImporter.h>
#include <SlateEngine\Engine\FileSystem\Importers\WICTextureFileImporter.h>
#include <SlateEngine\Engine\FileSystem\Importers\DDSTextureFileImporter.h>

static class AssetImporter {
public:
	static std::unordered_map<int, IAssetImporter*> importers;
};