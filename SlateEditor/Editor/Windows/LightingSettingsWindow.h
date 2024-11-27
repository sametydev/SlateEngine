#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Game/Game.h>

class LightingSettingsWindow :public IWindow
{
public:
	LightingSettingsWindow();
	~LightingSettingsWindow();

	void OnInit() override;
	void OnDraw(bool* closable = nullptr) override;
private:
	void DrawTransform(const char* label, vec3f& val);
	const char* windowName;
	Game* game = nullptr;
};

