#pragma once
#include <SlateEngine/Engine/Editor/Windows/IWindow.h>

class AssetsBrowser : public IWindow
{
public:
	AssetsBrowser();
	~AssetsBrowser();

	void OnInit() override;
	void OnDraw() override;
private:
	const char* windowName;
};

