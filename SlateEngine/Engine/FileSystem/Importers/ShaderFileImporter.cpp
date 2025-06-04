#include "ShaderFileImporter.h"
#include <inc/SimpleIni.h>
#include <SlateEngine\Engine\Graphics\Shader\ShaderInformation.h>
#include <SlateEngine\Engine\Graphics\Shader\ShaderCache.h>
#include <SlateEngine\Engine\Graphics\Vertex.h>

#pragma comment(lib, "rpcrt4.lib")

ShaderFileImporter::ShaderFileImporter()
{
}

ShaderFileImporter::~ShaderFileImporter()
{
}

void ShaderFileImporter::ImportAsset(FileSystem* fs, std::filesystem::path _p)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(_p.c_str());

    ShaderInformation sinfo{};
    sinfo.displayName = ini.GetValue("Shader", "DisplayName");
    sinfo.csoName = ini.GetValue("Shader", "CSOName");
    sinfo.hlslFile = ini.GetValue("Shader", "HLSLFile");
    sinfo.entryPoint = ini.GetValue("Shader", "EntryPoint");

    std::string shaderType(ini.GetValue("Shader", "ShaderType"));

    if (shaderType == "Pixel") {
        ShaderCache::CreatePixelShader(sinfo);
    }
    else if (shaderType == "Vertex") {
        sinfo.inputLayout = ini.GetValue("Shader", "InputLayout");
        std::string sInputLayout = sinfo.inputLayout;

        if (sInputLayout == "VertexPNT") {
            ShaderCache::CreateVertexShader(sinfo)->CreateInputLayout(VertexPNT::inputLayout, ARRAYSIZE(VertexPNT::inputLayout));
        }
        else if (sInputLayout == "VertexPC") {
            ShaderCache::CreateVertexShader(sinfo)->CreateInputLayout(VertexPC::inputLayout, ARRAYSIZE(VertexPC::inputLayout));
        }
    }
}
