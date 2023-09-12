#pragma once

#include <iostream>
#include <filesystem>

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	void Init();

	void ProcessScriptFile(std::filesystem::path _p);
	void ProcessTextureFileWIC(std::filesystem::path _p);
	void ProcessTextureFileDDS(std::filesystem::path _p);


	static FileSystem* Instance;
};

