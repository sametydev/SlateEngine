#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/Entity/EntityManager.h>

class InspectorWindow :
    public IWindow
{
public:
	InspectorWindow();
	~InspectorWindow();

	void OnInit() override;
	void OnDraw(bool* closable = nullptr) override;
	void OnDraw(Entity* entity);

	void DrawTransform(const char* label,vec3f& val);

	void DrawRenderableGeometryComponent(Entity* entity);
	void DrawLuaScriptComponent(Entity* entity);
private:
	const char* windowName;

	Game* game = nullptr;

	std::unordered_map<std::string, SLATE_UUID> scriptsMapBuffer;
	std::vector<std::string> scriptNames;
	std::vector<const char*> scriptNamePtrs;
};

