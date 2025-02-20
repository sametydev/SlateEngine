#include "FileSystemManager.h"
FileSystemManager* FileSystemManager::Instance = nullptr;

FileSystemManager::FileSystemManager()
{
    if (Instance == nullptr) {
        Instance = this;
    }
}

FileSystemManager::~FileSystemManager()
{
}

void FileSystemManager::CreateProjectFromDefaultTemplate(Project* data)
{
	std::string src = data->projectTemplate;

    if (data->projectTemplate.empty()) {
        src = "Projects/TestProject/";
    }

	std::string target = "Projects/" + data->projectPath;

    bool success = 0;

    try
    {
        success = fs::create_directory(target);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return;
    }

    try
    {
        fs::copy(src, target, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return;
    }

    success = 1;
}
