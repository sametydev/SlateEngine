#pragma once
#include "../Types.h"

namespace fs = std::filesystem;

class FileSystemManager
{
public:
	FileSystemManager();
	~FileSystemManager();

	static FileSystemManager* Instance;

	void LoadProject(/* TODO */);
	void DeleteProject(/* TODO */);
	void CreateProjectFromDefaultTemplate(Project* data);
};

