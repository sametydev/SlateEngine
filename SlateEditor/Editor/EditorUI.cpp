#include <SlateEditor/Editor/EditorUI.h>
#include <SlateEditor/Editor/Windows/LogWindow.h>
#include <SlateEditor/Editor/Windows/AssetsBrowser.h>
#include <SlateEditor/Editor/Windows/UIRenderablesWindow.h>
#include <SlateEditor/Editor/Windows/LightingSettingsWindow.h>
#include <entt.hpp>
#include <ImGuizmo.h>
#include <comdef.h>
#include <SlateEngine/Engine/Input/InputSystem.h>

EditorUI* EditorUI::Instance = nullptr;

EditorUI::EditorUI()
{
	if (!Instance)
	{
		Instance = this;
	}
}

EditorUI::~EditorUI()
{
	if (IS_COOKED) return;
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}



void EditorUI::OnInit()
{
	if (IS_COOKED) return;
    //Initialize IMGUI for Test
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(DXApplication::Instance->MainWnd());
    ImGui_ImplDX11_Init(DXApplication::Instance->GetDXDevice().Get(), DXApplication::Instance->GetDXContext().Get());
    

    InitTheme();

	game = Game::Instance;

    LogWindow* logWindow = new LogWindow();
    inspectorWindow = new InspectorWindow();
	sceneWindow = new SceneHierarchy();
    LightingSettingsWindow* light = new LightingSettingsWindow();
	AssetsBrowser* assetBrowser = new AssetsBrowser();


    windows.emplace(logWindow);
    windows.emplace(sceneWindow);
    windows.emplace(light);
	windows.emplace(assetBrowser);

    for (auto w : windows)
    {
        w->OnInit();
    }

	inspectorWindow->OnInit();

}

void EditorUI::NewFrame()
{
	if (IS_COOKED) return;
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
	ImGuizmo::BeginFrame();

}

#define DXInstance DXApplication::Instance

void EditorUI::OnUpdate(float deltaTime)
{
	if (IS_COOKED) return;
    static bool dockspaceOpen = true;


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

	if (io->ConfigFlags & ImGuiConfigFlags_DockingEnable)
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
			ImGui::MenuItem("Test", NULL);
			ImGui::EndMenu();
		}

	}
	ImGui::EndMenuBar();
	ImGui::End();

	constexpr float movementSpeed = 4.2f;
	constexpr float mouseSpeed = 0.1333f;

	if (ImGui::IsKeyDown(ImGuiKey_MouseRight)) {

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

	if (ImGui::Begin("Editor")) {
		DrawViewportMenu();

		ImGui::Image(m_viewportSRV.Get(), ImGui::GetContentRegionAvail());

		if (ImGui::IsKeyPressed(ImGuiKey_T))
		{
			gizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_R))
		{
			gizmoType = ImGuizmo::OPERATION::ROTATE;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_S))
		{
			gizmoType = ImGuizmo::OPERATION::SCALE;
		}


		if (sceneWindow->selectedEntity && gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			
			float w = ImGui::GetWindowWidth();
			//adding 50.f to height because in this calculation its calc all viewport window, not viewport texture, for temporary i need this like solution
			float h = ImGui::GetWindowHeight() + 50.f;

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, w, h);

			const mat4x4& cproj = mat4x4::transposed(game->m_camera->GetProjectionMatrix());
			const mat4x4& cview = mat4x4::transposed(game->m_camera->GetViewMatrix());

			Transform& tc = sceneWindow->selectedEntity->GetComponent<Transform>();

			mat4x4 t = mat4x4::transposed(tc.GetGlobal());

			float translation[3] = { tc.mPosition.x, tc.mPosition.y, tc.mPosition.z };
			float rotation[3] = { tc.mRotation.x, tc.mRotation.y, tc.mRotation.z };
			float scale[3] = { tc.mScale.x, tc.mScale.y, tc.mScale.z };

			ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, *t.m);
			ImGuizmo::Manipulate(cview.f, cproj.f, (ImGuizmo::OPERATION)gizmoType, ImGuizmo::LOCAL, *t.m);

			/*
			S R  T
			4 8  12
			5 9  13
			6 10 14
			*/

			if (ImGuizmo::IsUsing()) {
				float translation[3] = { 0.0f, 0.0f, 0.0f }, rotation[3] = { 0.0f, 0.0f, 0.0f }, scale[3] = { 0.0f, 0.0f, 0.0f };

				ImGuizmo::DecomposeMatrixToComponents(*t.m, translation, rotation, scale);

				tc.mPosition = vec3f(translation[0], translation[1], translation[2]);
				tc.mRotation = vec3f(rotation[0], rotation[1], rotation[2]);
				tc.mScale	 = vec3f(scale[0], scale[1], scale[2]);
			}
		}
	}

	
	ImGui::End();

    for (auto w : windows)
    {
        w->OnDraw();
    }

	inspectorWindow->OnDraw(sceneWindow->selectedEntity);

	if (ImGui::Begin("Render")) {
		ImGui::ColorEdit3("Clear Color", game->clear);

		ImGui::Checkbox("WireFrame Mode", &game->renderWireframe);

		_bstr_t gxd(DXApplication::Instance->adapterDesc.Description);

		const char* gcardDesc = gxd;
		ImGui::Text("Graphics Device : ");
		ImGui::Text(gcardDesc);

	}ImGui::End();

}

void EditorUI::ClearViewport(float rgba[4])
{
	DXInstance->GetDXContext()->OMSetRenderTargets(1, m_viewportRTV.GetAddressOf(), DXInstance->GetDepthStencilView().Get());
	DXInstance->GetDXContext()->ClearRenderTargetView(m_viewportRTV.Get(), rgba);
	DXInstance->GetDXContext()->ClearDepthStencilView(DXInstance->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void EditorUI::ResizeViewport(int w, int h)
{
	viewportW = w;
	viewportH = h;
	m_viewportTexture.Reset();
	m_viewportRTV.Reset();
	m_viewportSRV.Reset();

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = w;
	textureDesc.Height = h;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 4;
	textureDesc.SampleDesc.Quality = DXInstance->mMsaaQuality - 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (DXInstance->bEnableMsaa)
	{
		textureDesc.SampleDesc.Count = 4;
		textureDesc.SampleDesc.Quality = DXInstance->mMsaaQuality - 1;
	}
	else
	{
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
	}

	DXInstance->GetDXDevice()->CreateTexture2D(&textureDesc, NULL, &m_viewportTexture);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	DXInstance->GetDXDevice()->CreateRenderTargetView(m_viewportTexture.Get(), nullptr, &m_viewportRTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	DXInstance->GetDXDevice()->CreateShaderResourceView(m_viewportTexture.Get(), nullptr, &m_viewportSRV);

	DXInstance->GetDXContext()->OMSetRenderTargets(1, m_viewportRTV.GetAddressOf(), DXInstance->GetDepthStencilView().Get());
}

void EditorUI::DrawViewportMenu()
{
	if (ImGui::Button("PLAY")) { game->gameState = (GameState)1; }
	ImGui::SameLine();
	if (ImGui::Button("PAUSE")) { game->gameState = (GameState)2; }
	ImGui::SameLine();
	if (ImGui::Button("STOP")) { game->gameState = (GameState)0; }

	ImGui::SameLine();

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	if (ImGui::Button(" T ")) { gizmoType = ImGuizmo::OPERATION::TRANSLATE; }
	ImGui::PopStyleColor(1);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0.71f, 0.0f, 1.0f });
	if (ImGui::Button(" R ")) { gizmoType = ImGuizmo::OPERATION::ROTATE; }
	ImGui::PopStyleColor(1);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0.0f, 1.0f, 1.0f });
	if (ImGui::Button(" S ")) { gizmoType = ImGuizmo::OPERATION::SCALE; }
	ImGui::PopStyleColor(1);
}

void EditorUI::InitTheme()
{
	if (IS_COOKED) return;
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

}

void EditorUI::OnRender()
{
	if (IS_COOKED) return;
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}