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

            if (i->path().extension() == ".lua") {
                ProcessScriptFile(i->path());
            }

            else if (i->path().extension() == ".png" ||
                i->path().extension() == ".jpg"  ||
                i->path().extension() == ".jpeg" ||
                i->path().extension() == ".bmp"  ||
                i->path().extension() == ".tiff")
            {
                ProcessTextureFileWIC(i->path());
            }

            else if (i->path().extension() == ".dds") {
                ProcessTextureFileDDS(i->path());
            }
        }
    }
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

