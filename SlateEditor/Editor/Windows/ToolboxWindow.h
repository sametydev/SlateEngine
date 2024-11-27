#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Game/Game.h>

class ToolboxWindow :
	public IWindow
{
public:
	ToolboxWindow();
	~ToolboxWindow();

	void OnInit() override;
	void OnDraw(bool* closable = nullptr) override;
private:
	const char* windowName;
	Game* game;
public:
	int gizmoType = 7;
};
