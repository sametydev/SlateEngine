#pragma once
#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <SlateEditor/Editor/Windows/SceneHierarchy.h>
#include <SlateEditor/Editor/Windows/InspectorWindow.h>
#include <SlateEngine/Engine/Editor/EditorSystem.h>
#include <SlateEditor/Editor/Windows/AssetsBrowser.h>
#include <SlateEditor/Editor/Windows/LogWindow.h>
#include <SlateEditor/Editor/Windows/LightingSettingsWindow.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <SlateEngine/Engine/Graphics/Texture/RenderTTexture.h>
#include <set>
#include <SlateEngine/Engine/Input/Gamepad.h>

class EditorUI : public EditorSystem
{
public:
	EditorUI();
	~EditorUI();

	void OnInit(HWND wnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;

	void NewFrame() override;
	void OnRender() override;

	//Update first render later
	void OnUpdate(float deltaTime) override;

	void ClearViewport(float rgba[4]) override;

	void ResizeViewport(int w, int h) override;
	void HandleInput(float deltaTime);
	void DrawViewportMenu();

	static EditorUI* Instance;

private:
	friend class Game;
	friend class DXApplication;

	void InitTheme();

	Game* game = nullptr;

	ImGuiIO* io = nullptr;

	std::set<IWindow*> windows;

	InspectorWindow* inspectorWindow = nullptr;
	SceneHierarchy* sceneWindow      = nullptr;
	LogWindow* logWindow             = nullptr;
	LightingSettingsWindow* light    = nullptr;
	AssetsBrowser* assetBrowser      = nullptr;

	std::unique_ptr<RenderTTexture> rtt;

	int gizmoType = 7;

	std::unique_ptr<Gamepad> gamepad;
	Camera* mainCamera = nullptr;
};
