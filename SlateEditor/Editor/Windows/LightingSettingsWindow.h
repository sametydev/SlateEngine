#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Game/Game.h>

class LightingSettingsWindow :public IWindow
{
public:
	LightingSettingsWindow();
	~LightingSettingsWindow();

	void OnInit() override;
	void OnDraw() override;
private:
	const char* windowName;
	Game* game = nullptr;
};

