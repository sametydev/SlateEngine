#pragma once

#include <iostream>
#include <filesystem>

enum FILE_TYPE {
	MISC,
	SHADER,
	TEXTURE_WIC,
	TEXTURE_DDS,
	LUA
};

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	void Init();

	void InitFWatcher();

	FILE_TYPE GetFileTypeFromExt(std::filesystem::path ext);

	void ProcessScriptFile(std::filesystem::path _p);
	void ProcessTextureFileWIC(std::filesystem::path _p);
	void ProcessTextureFileDDS(std::filesystem::path _p);


	void ProcessMetaFileForTextures(std::filesystem::path _p);


	//Callbacks;
	void OnFileAdded(std::filesystem::path _p);
	void OnFileRemoved(std::filesystem::path _p);
	void OnFileModified(std::filesystem::path _p);
	void OnFileRenamedOld(std::filesystem::path oldName);
	void OnFileRenamedNew(std::filesystem::path newName);

	void ImportFile(std::filesystem::path _p);
	static FileSystem* Instance;

private:
	std::filesystem::path lastRemovedFile;
};