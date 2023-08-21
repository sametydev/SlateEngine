#pragma once
#include <SlateEngine/Engine/Core/Singleton.h>
#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Editor/Windows/SceneHierarchy.h>
#include <SlateEngine/Engine/Editor/Windows/InspectorWindow.h>
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

	void ClearViewport(float rgba[4]);

	void ResizeViewport(int w, int h);

private:
	friend class Game;
	friend class DXApplication;
	void InitTheme();

	ImGuiIO* io = nullptr;

	std::set<IWindow*> windows;
	InspectorWindow* inspectorWindow = nullptr;
	SceneHierarchy* sceneWindow = nullptr;

	ComPtr<ID3D11Texture2D> m_viewportTexture;
	ComPtr<ID3D11RenderTargetView> m_viewportRTV;
	ComPtr<ID3D11ShaderResourceView> m_viewportSRV;

	int viewportW, viewportH;
};
