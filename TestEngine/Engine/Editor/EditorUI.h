#pragma once
#include <TestEngine/Engine/Game/Game.h>
#include <TestEngine/Engine/Core/Singleton.h>
#include <TestEngine/Engine/DXConfig.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

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
	void InitTheme();
};
