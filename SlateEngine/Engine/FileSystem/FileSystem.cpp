#include "FileSystem.h"
#include <Windows.h>
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
}

void FileSystem::ProcessTextureFileDDS(std::filesystem::path _p)
{
    MessageBox(0, _p.c_str(), 0, 0);
}

