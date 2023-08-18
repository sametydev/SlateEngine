#pragma once
#include <SlateEngine/Engine/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Entity/EntityManager.h>

class SceneHierarchy :
	public IWindow
{
public:
	SceneHierarchy();
	~SceneHierarchy();

	void OnInit() override;
	void OnDraw(const char* title) override;
	void OnDraw() override;
private:
	const char* windowName;
};
