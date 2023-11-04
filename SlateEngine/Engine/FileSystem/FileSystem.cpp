#include "FileSystem.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <inc/SimpleIni.h>
#pragma comment(lib, "rpcrt4.lib")

FileSystem* FileSystem::Instance = nullptr;

FileSystem::FileSystem()
{
    if (!Instance)
    {
        Instance = this;
    }
}

FileSystem::~FileSystem()
{
}

void FileSystem::Init()
{
    for (std::filesystem::recursive_directory_iterator i("TestProject"), end; i != end; ++i) {
        if (!std::filesystem::is_directory(i->path())) {

            switch (GetFileTypeFromExt(i->path().extension()))
            {
                case FILE_TYPE::LUA:
                    ProcessScriptFile(i->path());
                    break;
                case FILE_TYPE::TEXTURE_WIC:
                    ProcessTextureFileWIC(i->path());
                    break;
                case FILE_TYPE::TEXTURE_DDS:
                    ProcessTextureFileDDS(i->path());
                    break;
            }
        }
    }
}

FILE_TYPE FileSystem::GetFileTypeFromExt(std::filesystem::path ext)
{
    if (ext == ".lua") {
        return FILE_TYPE::LUA;
    }
    else if (ext == ".png" ||
        ext == ".jpg" ||
        ext == ".jpeg" ||
        ext == ".bmp" ||
        ext == ".tiff")
    {
        return FILE_TYPE::TEXTURE_WIC;
    }
    else if (ext == ".dds") {
        return FILE_TYPE::TEXTURE_DDS;
    }
    return FILE_TYPE::MISC;
}

void FileSystem::ProcessScriptFile(std::filesystem::path _p)
{
}

void FileSystem::ProcessTextureFileWIC(std::filesystem::path _p)
{
    ProcessMetaFileForTextures(_p);
}

void FileSystem::ProcessTextureFileDDS(std::filesystem::path _p)
{
    ProcessMetaFileForTextures(_p);
}

void FileSystem::ProcessMetaFileForTextures(std::filesystem::path _p)
{
    std::string metaFile = _p.replace_extension().string() + ".smeta";

    if (!std::filesystem::exists(metaFile))
    {
        std::ofstream newMetaFile(metaFile);
        
        CSimpleIniA ini;
        ini.SetUnicode();

        UUID uuid;
        UuidCreate(&uuid);
        char* str;
        UuidToStringA(&uuid, (RPC_CSTR*)&str);
        
        ini.SetValue("Asset", "uuid", str);
        ini.SetValue("Asset", "path", _p.string().c_str());

        std::string data;
        ini.Save(data);

        newMetaFile << data;

        newMetaFile.close();

        RpcStringFreeA((RPC_CSTR*)&str);
    }
}

