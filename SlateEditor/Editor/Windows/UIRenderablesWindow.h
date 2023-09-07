#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Game/Game.h>


class UIRenderablesWindow :
	public IWindow
{
public:
	UIRenderablesWindow();
	~UIRenderablesWindow();

	void OnInit() override;
	void OnDraw() override;
private:
	const char* windowName;

	Game* game = nullptr;
	entt::entity selectedEntity;
};

