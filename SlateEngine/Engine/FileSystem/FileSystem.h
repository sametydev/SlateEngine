#pragma once

#include <iostream>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <SlateEngine/Engine/Utils.h>
#include <simdjson.h>

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
	FILE_TYPE ftype;
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
		return (FILE_TYPE)m_extensionLookupTable[ext.string()];
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
	void ProcessShaderFile(std::filesystem::path _p);
	void ProcessMetaFile(std::filesystem::path _p);

	void BuildExtensions();

	inline std::string GetExtFromP(std::filesystem::path _p)
	{
		return FTypeToString((FILE_TYPE)m_extensionLookupTable[_p.string()]);
	}

	//first is uuid, second is meta file path
	std::unordered_map<std::string, SMetaData> metaMap;

	std::map<std::string, UINT> m_extensionLookupTable;

	SMetaData* errorMetaData;
};