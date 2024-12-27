#include <SlateEditor/Editor/EditorUI.h>
#include <ImGuizmo.h>
#include <comdef.h>
#include <SlateEngine/Engine/Input/InputSystem.h>
#include <SlateEngine/Engine/NativeScripting/ScriptRegistry.h>


EditorUI* EditorUI::Instance = nullptr;

EditorUI::EditorUI()
{
	if (!Instance)
	{
		Instance = this;

		logWindow = new LogWindow();
		windows.emplace(logWindow);
	}
}

EditorUI::~EditorUI()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


void EditorUI::OnInit(HWND wnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{

	gamepad = std::make_unique<Gamepad>(1);
	gamepad->Update();
    ImGui::CreateContext();

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::GetIO().WantCaptureMouse = true;
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(wnd);
    ImGui_ImplDX11_Init(pDevice, pDeviceContext);
    

    InitTheme();

	cursorNormal					= LoadCursor(NULL, IDC_ARROW);
	cursorGrab						= LoadCursor(NULL, IDC_HAND);

	game							= Game::Instance;
	mainCamera						= game->m_camera;

	toolboxWindow					= new ToolboxWindow();
    inspectorWindow					= new InspectorWindow();
	sceneWindow						= new SceneHierarchy();
    light							= new LightingSettingsWindow();
	assetBrowser					= new AssetsBrowser();
	nativeScriptingDebuggerWindow	= new NativeScriptingDebugger();

	toolboxWindow->OnInit();
    windows.emplace(sceneWindow);
    windows.emplace(light);
	windows.emplace(assetBrowser);

    for (auto w : windows)
    {
        w->OnInit();
    }

	nativeScriptingDebuggerWindow->OnInit();
	inspectorWindow->OnInit();
	logWindow->AddLog("[Renderer] - DX11(DirectX 11_1) Renderer OnInit");
}

void EditorUI::NewFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

constexpr float movementSpeed = 4.2f;
constexpr float mouseSpeed = 0.1333f;

void EditorUI::HandleInput(float deltaTime) {
	if (ImGui::IsKeyDown(ImGuiKey_MouseRight) && game->GetGameState() == NONE) {
		::SetCursor(cursorGrab);

		vec2f delta = InputSystem::delta;

		game->m_camera->mRot.y += delta.x * mouseSpeed;
		game->m_camera->mRot.x += delta.y * mouseSpeed;

		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			game->m_camera->mPos += game->m_camera->mForward * movementSpeed * deltaTime;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			game->m_camera->mPos -= game->m_camera->mForward * movementSpeed * deltaTime;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			game->m_camera->mPos += game->m_camera->mRight * movementSpeed * deltaTime;
		}
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			game->m_camera->mPos -= game->m_camera->mRight * movementSpeed * deltaTime;
		}
	}
	else
	{
		if (ImGui::IsKeyPressed(ImGuiKey_T))
		{
			toolboxWindow->gizmoType = 7;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_R))
		{
			toolboxWindow->gizmoType = 120;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_S))
		{
			toolboxWindow->gizmoType = 896;
		}
	}
}

void EditorUI::OnUpdate(float deltaTime)
{
    static bool dockspaceOpen = true;

	if (gamepad->IsConnected()) {
		if (gamepad->IsButtonPressed(SLATE_GAMEPAD_A)) {
			MessageBoxA(0, "Gamepad A Button Pressed", 0, 0);
		}
		if (gamepad->IsButtonPressed(SLATE_GAMEPAD_B)) {
			gamepad->Vibrate(25000);
		}
		if (gamepad->IsButtonPressed(SLATE_GAMEPAD_Y)) {
			gamepad->ResetVibration();
		}
	}

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	ImGui::PopStyleVar(2);

	if (ImGui::BeginMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Test", NULL);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Add Empty Entity", NULL)) {
				EntityManager::Instance->RegisterEntity(new Entity());
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if(ImGui::MenuItem("Native Scripting Debugger", NULL))
			{
				nativeScriptingDebugger_Open = true;
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Run")) {
			if (ImGui::MenuItem("Play Game on Runner (External)", NULL)) {
				ShellExecuteA(NULL, "open", "bin\\SlatePlayer.exe", gDXApp->GetWorkingDir().c_str(), NULL, SW_SHOWDEFAULT);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Plugins"))
		{
			ImGui::MenuItem("Test", NULL);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::MenuItem("Test", NULL);
			ImGui::EndMenu();
		}
	}
	ImGui::EndMenuBar();
	ImGui::End();
	

	if (ImGui::Begin("Editor")) {
		if (ImGui::IsWindowFocused()) {
			HandleInput(deltaTime);
		}

		toolboxWindow->OnDraw();
		
		ImVec2 win_region = ImGui::GetContentRegionAvail();

		mainCamera->SetAspectRatio(win_region.x / win_region.y);
		game->FrameBufferConstantObject.proj = mainCamera->GetProjectionMatrix();

		ImGui::Image(Game::Instance->GetRenderTarget()->mRenderPass[0]->GetShaderResourceView(), win_region);
		
		/*
		float originalWidth = rtt->GetWidth();
		float originalHeight = rtt->GetHeight();

		ImVec2 availableRegion = ImGui::GetContentRegionAvail();
		float regionWidth = availableRegion.x;
		float regionHeight = availableRegion.y;

		float aspectRatio = originalWidth / originalHeight;
		float regionAspectRatio = regionWidth / regionHeight;

		float drawWidth, drawHeight;
		if (regionAspectRatio > aspectRatio) {
			drawHeight = regionHeight;
			drawWidth = drawHeight * aspectRatio;
		}
		else {
			drawWidth = regionWidth;
			drawHeight = drawWidth / aspectRatio;
		}
		ImGui::Image(&rtt->GetShaderResourceView(), ImVec2(drawWidth, drawHeight));
		*/
		if (sceneWindow->selectedEntity && toolboxWindow->gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			
			float w = ImGui::GetWindowWidth();
			float h = ImGui::GetWindowHeight();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, w, h);

			mat4x4 cproj = mat4x4::transposed(game->m_camera->GetProjectionMatrix());
			mat4x4 cview = mat4x4::transposed(game->m_camera->GetViewMatrix());
			Transform& tc = sceneWindow->selectedEntity->GetComponent<Transform>();


			float translation[3]	= { tc.mPosition.x, tc.mPosition.y, tc.mPosition.z };
			float rotation[3]		= { tc.mRotation.x, tc.mRotation.y, tc.mRotation.z };
			float scale[3]			= { tc.mScale.x,	tc.mScale.y,	tc.mScale.z };


			float t2[16];

			ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, t2);
			ImGuizmo::Manipulate(cview, cproj, (ImGuizmo::OPERATION)toolboxWindow->gizmoType, ImGuizmo::WORLD,t2);

			/*
			S R  T
			4 8  12
			5 9  13
			6 10 14
			*/

			if (ImGuizmo::IsUsing()) {
				float translation[3] = { 0.0f, 0.0f, 0.0f }, rotation[3] = { 0.0f, 0.0f, 0.0f }, scale[3] = { 0.0f, 0.0f, 0.0f };

				ImGuizmo::DecomposeMatrixToComponents(t2, translation, rotation, scale);

				tc.mPosition	= vec3f(0[translation], 1[translation],		2[translation]);
				tc.mRotation	= vec3f(0[rotation],	1[rotation],		2[rotation]);
				tc.mScale		= vec3f(0[scale],		1[scale],			2[scale]);
			}
		}
	}

	
	ImGui::End();

    for (auto w : windows)
    {
        w->OnDraw();
    }

	inspectorWindow->OnDraw(sceneWindow->selectedEntity);
	nativeScriptingDebuggerWindow->OnDraw(&nativeScriptingDebugger_Open);
	if (ImGui::Begin("Render")) {
		ImGui::ColorEdit3("Clear Color", game->clear);

		//ImGui::Checkbox("WireFrame Mode", &game->renderWireframe);

		//ImGui::Text("Graphics Device : %s", HWInfo::gpuName_cstr);

		//ImGui::Text("SSE Support : %s", HWInfo::sseSupported ? "Yes" : "False");

		ImGui::Text("FPS : %f", Timer::Instance->fps);
		ImGui::Text("Draw Call : %f", DXApplication::Instance->pipelineStatics.CPrimitives);


	}ImGui::End();

}

void EditorUI::ClearViewport(float rgba[4])
{
}

void EditorUI::ResizeViewport(int w, int h)
{
}

LRESULT EditorUI::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;
}

void EditorUI::InitTheme()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.5f;
	style.WindowPadding = ImVec2(13.0f, 10.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 3.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 5.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(20.0f, 8.100000381469727f);
	style.FrameRounding = 2.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(3.0f, 3.0f);
	style.ItemInnerSpacing = ImVec2(3.0f, 8.0f);
	style.CellPadding = ImVec2(6.0f, 14.10000038146973f);
	style.IndentSpacing = 0.0f;
	style.ColumnsMinSpacing = 10.0f;
	style.ScrollbarSize = 10.0f;
	style.ScrollbarRounding = 2.0f;
	style.GrabMinSize = 12.10000038146973f;
	style.GrabRounding = 1.0f;
	style.TabRounding = 2.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 5.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.9803921580314636f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09411764889955521f, 0.09411764889955521f, 0.09411764889955521f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.09411764889955521f, 0.09411764889955521f, 0.09411764889955521f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.09803921729326248f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.09803921729326248f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1568627506494522f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0470588244497776f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1176470592617989f, 0.1176470592617989f, 0.1176470592617989f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1176470592617989f, 0.1176470592617989f, 0.1176470592617989f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.1098039224743843f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.0f, 1.0f, 1.0f, 0.3921568691730499f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.4705882370471954f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.09803921729326248f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 0.3921568691730499f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.3137255012989044f);
	style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 0.09803921729326248f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1568627506494522f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0470588244497776f);
	style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 1.0f, 1.0f, 0.09803921729326248f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1568627506494522f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0470588244497776f);
	style.Colors[ImGuiCol_Separator] = ImVec4(1.0f, 1.0f, 1.0f, 0.1568627506494522f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.2352941185235977f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.2352941185235977f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 1.0f, 1.0f, 0.1568627506494522f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.2352941185235977f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.2352941185235977f);
	style.Colors[ImGuiCol_Tab] = ImVec4(1.0f, 1.0f, 1.0f, 0.09803921729326248f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1568627506494522f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.3137255012989044f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.0f, 0.0f, 0.1568627506494522f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.2352941185235977f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 1.0f, 1.0f, 0.3529411852359772f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 1.0f, 1.0f, 0.3529411852359772f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0f, 1.0f, 1.0f, 0.3137255012989044f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0f, 1.0f, 1.0f, 0.196078434586525f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.01960784383118153f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.168627455830574f, 0.2313725501298904f, 0.5372549295425415f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5647059082984924f);
}

void EditorUI::OnRender(float rgba[4])
{
    ImGui::Render();
	ImGui::UpdatePlatformWindows();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::RenderPlatformWindowsDefault();
}
