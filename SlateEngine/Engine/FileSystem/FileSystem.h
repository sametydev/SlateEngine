#pragma once

#include <iostream>
#include <filesystem>
#include <unordered_map>

enum FILE_TYPE {
	MISC,
	SHADER,
	TEXTURE_WIC,
	TEXTURE_DDS,
	LUA,
	MESH,
	AUDIO,
	SMETA
};

struct SMetaData {
	std::string uuid;
	std::string path;
	FILE_TYPE ftype;
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
	void OnFileAdded(std::filesystem::path				 _p);
	void OnFileRemoved(std::filesystem::path			 _p);
	void OnFileModified(std::filesystem::path			 _p);
	void OnFileRenamedOld(std::filesystem::path		oldName);
	void OnFileRenamedNew(std::filesystem::path		newName);

	std::string GetUUIDFromFPath(std::filesystem::path _p);
	SMetaData& GetSMetaDataFromFPath(std::filesystem::path _p);

	static FileSystem* Instance;

	std::unordered_map<std::string, SMetaData>& GetMetaMap() { return metaMap; }

	inline static const char* FTypeToString(FILE_TYPE v)
	{
		switch (v)
		{
			case MISC:			return "MISC";
			case SHADER:		return "SHADER";
			case TEXTURE_WIC:	return "WIC Texture";
			case TEXTURE_DDS:	return "DDS Texture";
			case LUA:			return "LUA";
			case MESH:			return "MESH";
			case AUDIO:			return "AUDIO";
			case SMETA:			return "SlateMeta File";
			default:			return "[Unknown File Type]";
		}
	}

private:
	std::vector
	<std::filesystem::path> lastRemovedFiles;

private:
	friend class AssetStreamer;
	void InitFWatcher();
	void ImportFile(std::filesystem::path _p);
	void ProcessScriptFile(std::filesystem::path _p);
	void ProcessTextureFileWIC(std::filesystem::path _p);
	void ProcessTextureFileDDS(std::filesystem::path _p);
	void ProcessMetaFile(std::filesystem::path _p);

	inline std::string GetExtFromP(std::filesystem::path _p)
	{
		if (_p.extension() == ".lua") {
			return "LUA";
		}
		else if (_p.extension() == ".png"	||
			_p.extension() == ".jpg"		||
			_p.extension() == ".jpeg"		||
			_p.extension() == ".bmp"		||
			_p.extension() == ".tiff")
		{
			return "TEXTURE";
		}
		else if (_p.extension() == ".dds") {
			return "TEXTURE";
		}
		return "MISC";
	}

	//first is uuid, second is meta file path
	std::unordered_map<std::string, SMetaData> metaMap;

	SMetaData* errorMetaData;
};