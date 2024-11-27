#include "AssetsBrowser.h"
#include <SlateEngine/Engine/FileSystem/FileSystem.h>

static std::filesystem::path assetPath = "TestProject\\Assets";

AssetsBrowser::AssetsBrowser() : windowName("Asset Browser")
{
	currentDir = assetPath;
}

AssetsBrowser::~AssetsBrowser()
{
}

void AssetsBrowser::OnInit()
{
}


void AssetsBrowser::OnDraw(bool* closable)
{
	ImGui::Begin(windowName);
	ImGui::Text("Current Path : %s", currentDir.string().c_str());
	ImGui::Dummy(ImVec2(10.f,10.f));

	if (currentDir != std::filesystem::path(assetPath))
	{
		if (ImGui::Button(" < ")) currentDir = currentDir.parent_path();
	}

	for (auto& i : std::filesystem::directory_iterator(currentDir))
	{
		if (i.is_directory())
		{
			if (ImGui::Button(i.path().filename().string().c_str())) currentDir /= i.path().filename();
		}
		else
		{
			if (!(i.path().extension() == ".smeta")) {
				if (ImGui::Button(i.path().filename().string().c_str())) {
					selectedFile = i.path();

					if (i.path().extension() == ".lua")
					{
						std::string x = std::filesystem::current_path().u8string()+"\\";
						std::string y = x+(i.path().u8string());

						//gDXApp->GetLogger()->AddLog(y.c_str());

						ShellExecuteA(NULL, "open", y.c_str(), NULL, NULL, SW_SHOWDEFAULT);

					}
				};
			}
		}

	}
	ImGui::End();


	ImGui::Begin("File Inspector");
		if (selectedFile != "")
		{
			ImGui::Dummy(ImVec2(3.f, 3.f));
			ImGui::Text("File Path : %s", selectedFile.string().c_str());
			SMetaData smd = FileSystem::Instance->GetSMetaDataFromFPath(selectedFile);
			ImGui::Text("File Type : %s",FileSystem::FTypeToString(smd.ftype));
			ImGui::Text("UUID : %s", smd.uuid.c_str());
		};
	ImGui::End();

}
