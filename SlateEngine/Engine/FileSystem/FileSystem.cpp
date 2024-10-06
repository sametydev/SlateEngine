#include "FileSystem.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <inc/SimpleIni.h>
#include <inc/FileWatcher.hpp>
#pragma comment(lib, "rpcrt4.lib")
#include <SlateEngine/Engine/Core/EngineConfig.h>

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
    errorMetaData = new SMetaData();
    errorMetaData->uuid = "ERROR";
    errorMetaData->path = "ERROR";

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

                FileSystem::Instance->lastRemovedFiles.push_back(path);
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

std::string FileSystem::GetUUIDFromFPath(std::filesystem::path _p)
{
    for (auto& i : metaMap)
    {
        if (i.second.path == _p)
        {
            return i.first;
        }
    }
    return "";
}

SMetaData& FileSystem::GetSMetaDataFromFPath(std::filesystem::path _p)
{
    std::string xp = _p.string() + ".smeta";
    for (auto& i : metaMap)
    {
        if (i.second.path == xp)
        {
            return i.second;
        }
    }

    return *errorMetaData;
}

void FileSystem::ProcessScriptFile(std::filesystem::path _p)
{
    ProcessMetaFile(_p);
}

void FileSystem::ProcessTextureFileWIC(std::filesystem::path _p)
{
    ProcessMetaFile(_p);
}

void FileSystem::ProcessTextureFileDDS(std::filesystem::path _p)
{
    ProcessMetaFile(_p);
}

void FileSystem::ProcessMetaFile(std::filesystem::path _p)
{
    std::filesystem::path xp = _p;
    std::string metaFile = xp.string() + META_FILE_EXT;

    CSimpleIniA ini;
    ini.SetUnicode();

    if (!std::filesystem::exists(metaFile))
    {
        std::ofstream newMetaFile(metaFile);

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
        ini.SetValue("Asset", "type", GetExtFromP(_p).c_str());
        ini.SetValue("Asset", "path", _p.string().c_str());

        switch (GetFileTypeFromExt(_p.extension()))
        {
            case FILE_TYPE::TEXTURE_DDS:
                break;
        }

        std::string data;
        ini.Save(data);

        newMetaFile << data;

        newMetaFile.close();

        RpcStringFreeA((RPC_CSTR*)&str);
    }

    ini.LoadFile(metaFile.c_str());
    
    SMetaData smd;
    smd.ftype = GetFileTypeFromExt(_p.extension());
    smd.path = metaFile;
    smd.uuid = ini.GetValue("Asset", "uuid");
    metaMap.emplace(smd.uuid, smd);
}

void FileSystem::OnFileAdded(std::filesystem::path _p)
{
    if (!std::filesystem::is_directory(_p)) {
        if (std::count(lastRemovedFiles.begin(), lastRemovedFiles.end(), _p.filename()) > 0)
        {
            // Probably (?) this file is moved!
            // Actually in this method, im not sure file is %100 moved but, generally works xD

            //TODO :  Implement this;
            /*for (auto& meta : metaMap) {
                if (meta.second.path == lastRemovedFile.string())
                {

                }
            }*/


            //Removing element in last removed files
            auto it = std::find(lastRemovedFiles.begin(), lastRemovedFiles.end(),
                _p.filename());

            if (it != lastRemovedFiles.end()) {
                lastRemovedFiles.erase(it);
            }
        }
        else
        {
            ImportFile(_p);
        }
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

