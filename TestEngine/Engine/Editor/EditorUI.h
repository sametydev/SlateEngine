#pragma once
#include <TestEngine/Engine/Core/Singleton.h>
#include <TestEngine/Engine/Game/Game.h>
#include <TestEngine/Engine/DXConfig.h>
#include <TestEngine/Engine/Graphics/DXApplication.h>
#include <TestEngine/Engine/Editor/Windows/IWindow.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <set>

class EditorUI : public Singleton<EditorUI>
{
public:
	EditorUI();
	~EditorUI();

	void OnInit();

	void NewFrame();
	void OnRender();

	//Update first render later
	void OnUpdate();

private:
	friend class Game;
	void InitTheme();

	ImGuiIO* io = nullptr;

	std::set<IWindow*> windows;
};
