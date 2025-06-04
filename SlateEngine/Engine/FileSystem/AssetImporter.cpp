#include "AssetImporter.h"

std::unordered_map<int, IAssetImporter*> AssetImporter::importers = {
    { FILE_TYPE::SHADER, new ShaderFileImporter() },
    { FILE_TYPE::LUA, new LuaScriptFileImporter() },
    { FILE_TYPE::TEXTURE_WIC, new WICTextureFileImporter() },
    { FILE_TYPE::TEXTURE_DDS, new DDSTextureFileImporter() },


    { FILE_TYPE::MISC, new EmptyFileImporter() },
    { FILE_TYPE::MESH, new EmptyFileImporter() },
    { FILE_TYPE::AUDIO, new EmptyFileImporter() },
    { FILE_TYPE::SMETA, new EmptyFileImporter() },

};