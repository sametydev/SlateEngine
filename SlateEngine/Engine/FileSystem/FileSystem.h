#pragma once

#include <iostream>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <SlateEngine/Engine/Utils.h>
#include <simdjson.h>
#include <SlateEngine/Engine/Core/Project.h>

using SLATE_UUID = std::string;
using SLATE_PATH = std::string;

enum FILE_TYPE : int {
	LUA = 0,
	TEXTURE_WIC,
	TEXTURE_DDS,
	SHADER,
	MISC,
	MESH,
	AUDIO,
	SMETA
};

struct SMetaData {
	std::string uuid;
	std::string path;
	std::string metaPath;
	FILE_TYPE ftype;
};

struct SlateFileSystemContainer {
	std::unordered_map<FILE_TYPE, std::unordered_map<SLATE_UUID, SMetaData>> leafs;
};

struct SlateUUIDType {
	SLATE_UUID uuid;
	FILE_TYPE type;
};

class ENGINE_API FileSystem
{
public:
	FileSystem();
	~FileSystem();

	void Init();

	void LateInit();

	inline FILE_TYPE GetFileTypeFromExt(std::filesystem::path ext)
	{
		if (m_extensionLookupTable.count(ext.string()) == 0) {
			return FILE_TYPE::MISC;
		}
		return (FILE_TYPE)m_extensionLookupTable[ext.string()];
	}

	//Callbacks;
	void OnFileAdded(std::filesystem::path				 _p);
	void OnFileRemoved(std::filesystem::path			 _p);
	void OnFileModified(std::filesystem::path			 _p);
	void OnFileRenamedOld(std::filesystem::path		oldName);
	void OnFileRenamedNew(std::filesystem::path		newName);

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

	void ProcessMetaFile(std::filesystem::path _p);

private:
	std::vector
	<std::filesystem::path> lastRemovedFiles;

private:
	friend class AssetStreamer;
	void InitFWatcher();

	void BuildExtensions();

	inline std::string GetExtFromP(std::filesystem::path _p)
	{
		return FTypeToString((FILE_TYPE)m_extensionLookupTable[_p.string()]);
	}

	//they are pair
	std::unordered_map<SLATE_UUID, SMetaData> metaMap;
	std::unordered_map<SLATE_PATH, SlateUUIDType> metaPathMap;

	std::map<std::string, UINT> m_extensionLookupTable;

	SMetaData* errorMetaData;
};