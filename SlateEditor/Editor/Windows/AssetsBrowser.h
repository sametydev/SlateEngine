#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <filesystem>

class AssetsBrowser : public IWindow
{
public:
	AssetsBrowser();
	~AssetsBrowser();

	void OnInit() override;
	void OnDraw(bool* closable = nullptr) override;
private:
	const char* windowName;
	std::filesystem::path currentDir;
	std::filesystem::path selectedFile = "";
};

