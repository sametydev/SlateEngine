#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>

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

