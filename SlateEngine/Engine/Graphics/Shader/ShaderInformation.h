#pragma once
#include <SimpleIni.h>

struct ShaderInformation
{
public:
	const char* displayName;
	const char* shaderType;
	const char* inputLayout;
	const char* entryPoint;
	const char* hlslFile;
	const char* csoName;
};

class ENGINE_API ShaderInformationLoader {
public:
	static ShaderInformation LoadSInfo(const char* file) {
		ShaderInformation sinfo{};

		CSimpleIniA ini;
		ini.SetUnicode();

		SI_Error rc = ini.LoadFile(file);
		if (rc < 0) { MessageBox(0, L"ShaderInformation file is broken.", L"ShaderInformation", 0); };

		sinfo.csoName     = ini.GetValue("Shader", "CSOName");
		sinfo.hlslFile    = ini.GetValue("Shader", "HLSLFile");
		sinfo.entryPoint  = ini.GetValue("Shader", "EntryPoint");
		sinfo.inputLayout = ini.GetValue("Shader", "InputLayout");
		sinfo.shaderType  = ini.GetValue("Shader", "ShaderType");
		sinfo.displayName = ini.GetValue("Shader", "DisplayName");


		return sinfo;
	}
};
