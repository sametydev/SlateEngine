#include "FileSystem.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <inc/SimpleIni.h>
#include <inc/FileWatcher.hpp>
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
    InitFWatcher();

    for (std::filesystem::recursive_directory_iterator i("TestProject"), end; i != end; ++i) {
        if (!std::filesystem::is_directory(i->path())) {
            ImportFile(i->path());
        }
    }

}

void FileSystem::InitFWatcher()
{
    
    filewatch::FileWatch<std::wstring>* watch = new filewatch::FileWatch<std::wstring>(
        L"TestProject",
        [](const std::wstring& path, const filewatch::Event change_type) {

            switch (change_type)
            {
            case filewatch::Event::added:
                FileSystem::Instance->OnFileAdded(path);
                break;
            case filewatch::Event::removed:
                FileSystem::Instance->OnFileRemoved(path);

                FileSystem::Instance->lastRemovedFile = path;
                break;
            case filewatch::Event::modified:
                FileSystem::Instance->OnFileModified(path);
                break;
            case filewatch::Event::renamed_old:
                FileSystem::Instance->OnFileRenamedOld(path);
                break;
            case filewatch::Event::renamed_new:
                FileSystem::Instance->OnFileRenamedNew(path);
                break;
            };
        }
    );
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
        (void)UuidCreate(&uuid);
        char* str;
        (void)UuidToStringA(&uuid, (RPC_CSTR*)&str);
        
        /*
        SMeta (SlateEngine Meta File) Example;
            [Asset]
            uuid = UUID_PARAM
            type = TYPE_OF_ASSET (Ex: TEXTURE)
            path = PATH_OF_FILE
        */

        ini.SetValue("Asset", "uuid", str);
        ini.SetValue("Asset", "type", "TEXTURE");
        ini.SetValue("Asset", "path", _p.string().c_str());

        std::string data;
        ini.Save(data);
        newMetaFile << data;

        newMetaFile.close();

        RpcStringFreeA((RPC_CSTR*)&str);
    }
}

void FileSystem::OnFileAdded(std::filesystem::path _p)
{
    if (!std::filesystem::is_directory(_p)) {
        if (_p.filename() == lastRemovedFile.filename())
        {
            // Probably (?) this file is moved!
            // Actually in this method, im not sure file is %100 moved but, generally works xD
        }
        else
        {
            ImportFile(_p);
        }
        lastRemovedFile = "";
    }
}

void FileSystem::OnFileRemoved(std::filesystem::path _p)
{
}

void FileSystem::OnFileModified(std::filesystem::path _p)
{
}

void FileSystem::OnFileRenamedOld(std::filesystem::path oldName)
{
}

void FileSystem::OnFileRenamedNew(std::filesystem::path newName)
{
}

void FileSystem::ImportFile(std::filesystem::path _p)
{
    switch (GetFileTypeFromExt(_p.extension()))
    {
        case FILE_TYPE::LUA:
            ProcessScriptFile(_p);
            break;
        case FILE_TYPE::TEXTURE_WIC:
            ProcessTextureFileWIC(_p);
            break;
        case FILE_TYPE::TEXTURE_DDS:
            ProcessTextureFileDDS(_p);
            break;
    }
}

