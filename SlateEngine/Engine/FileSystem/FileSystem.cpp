#include "FileSystem.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <inc/SimpleIni.h>
#include <inc/FileWatcher.hpp>
#pragma comment(lib, "rpcrt4.lib")
#include <SlateEngine/Engine/Core/EngineConfig.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderCache.h>
#include <SlateEngine/Engine/Graphics/Vertex.h>
#include <stdexcept>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include "AssetImporter.h"

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
    BuildExtensions();

    errorMetaData = new SMetaData();
    errorMetaData->uuid = "";
    errorMetaData->path = "";

    InitFWatcher();

    simdjson::dom::parser parser;
    simdjson::dom::element doc;
    
    auto error = parser.load(PathMaker::Make(gDXApp->GetWorkingDir(),"Project.json")).get(doc);
    if (error) {
        throw std::runtime_error("Project json load failure");
    }
    
    Project* proj = new Project();

    std::string_view projectName;
    std::string_view projectVersion;
    uint64_t projectVsync;
    

    doc["Project"]["name"].get(projectName);
    doc["Project"]["version"].get(projectVersion);
    doc["Project"]["vsync"].get(projectVsync);

    Project::Instance->SetProjectName(std::string(projectName));
    Project::Instance->SetProjectVersion(std::string(projectVersion));
    Project::Instance->SetProjectVSYNC(projectVsync);
}

void FileSystem::LateInit()
{
    for (std::filesystem::recursive_directory_iterator i(gDXApp->GetWorkingDir()), end; i != end; ++i) {
        if (!std::filesystem::is_directory(i->path())) {
            AssetImporter::importers[GetFileTypeFromExt(i->path().extension())]->ImportAsset(this, i->path());
        }
    }
}

void FileSystem::InitFWatcher()
{
    filewatch::FileWatch<std::wstring>* watch = new filewatch::FileWatch<std::wstring>(
        s2ws(gDXApp->GetWorkingDir()),
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

SMetaData& FileSystem::GetSMetaDataFromFPath(std::filesystem::path _p)
{
    return metaMap[metaPathMap[_p.string()].uuid];
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

        std::string data;
        ini.Save(data);

        newMetaFile << data;

        newMetaFile.close();

        RpcStringFreeA((RPC_CSTR*)&str);
    }

    ini.LoadFile(metaFile.c_str());
    
    SMetaData smd;
    smd.ftype = GetFileTypeFromExt(_p.extension());
    smd.path = xp.string();
    smd.metaPath = metaFile;
    smd.uuid = ini.GetValue("Asset", "uuid");
    metaMap.emplace(smd.uuid, smd);

    SlateUUIDType uuid_type;
    uuid_type.type = GetFileTypeFromExt(_p.extension());
    uuid_type.uuid = smd.uuid;

    metaPathMap.emplace(xp.string(), uuid_type);
}

void FileSystem::BuildExtensions()
{
    if (m_extensionLookupTable.size() > 0) m_extensionLookupTable.clear();

    int id = iota(true);

    /*
    Lua = 0
    WIC Textures = 1
    DDS = 2
    SINFO(Shader) = 3
    */

    m_extensionLookupTable.insert({ ".lua",     id });
    iota();

    id = iota();
    m_extensionLookupTable.insert({ ".png",     id });
    m_extensionLookupTable.insert({ ".jpg",     id });
    m_extensionLookupTable.insert({ ".jpeg",    id });
    m_extensionLookupTable.insert({ ".bmp",     id });
    m_extensionLookupTable.insert({ ".tiff",    id });

    m_extensionLookupTable.insert({ ".dds",     iota() });
    m_extensionLookupTable.insert({ ".sinfo",   iota() });
    iota(true);
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
            //Import assets
            AssetImporter::importers[GetFileTypeFromExt(_p.extension())]->ImportAsset(this, _p);
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

