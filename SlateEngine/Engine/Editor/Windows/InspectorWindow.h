#pragma once
#include <SlateEngine/Engine/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/Entity/EntityManager.h>

class InspectorWindow :
    public IWindow
{
public:
	InspectorWindow();
	~InspectorWindow();

	void OnInit() override;
	void OnDraw(const char* title) override;
	void OnDraw() override;
	void OnDraw(Entity* entity);

	void DrawTransform(const char* label,vec3f& val);
private:
	const char* windowName;

	Game* game = nullptr;
};

