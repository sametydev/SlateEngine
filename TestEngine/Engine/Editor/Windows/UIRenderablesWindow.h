#pragma once
#include <TestEngine/Engine/Editor/Windows/IWindow.h>
#include <TestEngine/Engine/Game/Game.h>

class UIRenderablesWindow :
	public IWindow
{
public:
	UIRenderablesWindow();
	~UIRenderablesWindow();

	void OnInit() override;
	void OnDraw(const char* title) override;
	void OnDraw() override;
private:
	const char* windowName;

	Game* game = nullptr;
};

