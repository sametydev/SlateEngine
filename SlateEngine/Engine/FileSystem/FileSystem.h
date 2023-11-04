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

	inline FILE_TYPE GetFileTypeFromExt(std::filesystem::path ext)
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

	//Callbacks;
	void OnFileAdded(std::filesystem::path _p);
	void OnFileRemoved(std::filesystem::path _p);
	void OnFileModified(std::filesystem::path _p);
	void OnFileRenamedOld(std::filesystem::path oldName);
	void OnFileRenamedNew(std::filesystem::path newName);

	static FileSystem* Instance;

private:
	std::filesystem::path lastRemovedFile;

private:
	void InitFWatcher();
	void ImportFile(std::filesystem::path _p);
	void ProcessScriptFile(std::filesystem::path _p);
	void ProcessTextureFileWIC(std::filesystem::path _p);
	void ProcessTextureFileDDS(std::filesystem::path _p);
	void ProcessMetaFileForTextures(std::filesystem::path _p);


};