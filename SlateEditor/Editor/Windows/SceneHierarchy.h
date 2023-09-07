#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Entity/EntityManager.h>

class SceneHierarchy :
	public IWindow
{
public:
	SceneHierarchy();
	~SceneHierarchy();

	void OnInit() override;
	void OnDraw() override;

	Entity* selectedEntity = nullptr;

private:
	const char* windowName;
};
