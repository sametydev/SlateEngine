#pragma once
#include <SlateEngine/Engine/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Game/Game.h>

class LightingSettingsWindow :public IWindow
{
public:
	LightingSettingsWindow();
	~LightingSettingsWindow();

	void OnInit() override;
	void OnDraw(const char* title) override;
	void OnDraw() override;
private:
	const char* windowName;
	Game* game = nullptr;
};

