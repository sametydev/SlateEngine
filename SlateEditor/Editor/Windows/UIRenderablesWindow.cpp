#include <SlateEditor/Editor/Windows/UIRenderablesWindow.h>

UIRenderablesWindow::UIRenderablesWindow()
{
}

UIRenderablesWindow::~UIRenderablesWindow()
{
}

void UIRenderablesWindow::OnInit()
{
	windowName = "UI Renderables";
	game = Game::Instance;
}

#define EntityRegistar D2DContext::Instance->GetRegistar()

float ui_x, ui_y;
void UIRenderablesWindow::OnDraw()
{
	ImGui::Begin(windowName);


	ImGui::Text("UI Text Summoner:");

	ImGui::InputFloat("Text X", &ui_x);
	ImGui::InputFloat("Text Y", &ui_y);

	static char bufpass[64] = "MyLabel";
	ImGui::InputText("Label", bufpass, 64);
	if (ImGui::Button("Add Text To Render"))
	{
		D2DContext::Instance->AddTextForRender(charToWChar(bufpass), ui_x, ui_y, 600.0f, 200.0f);
	}

	if (ImGui::Button("Remove"))
	{
		if (EntityRegistar.valid(selectedEntity)) {
			EntityRegistar.destroy(selectedEntity);
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 15.0f));
	ImGui::Text("2D Renderables List : ");
	EntityRegistar.each([&](auto entity)
		{
			C2DText& name = EntityRegistar.get<C2DText>(entity);

			ImGuiTreeNodeFlags flags = ((selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

			if (ImGui::TreeNodeEx(name.GetText(),flags))
			{
				ImGui::TreePop();
				
			}
			if (ImGui::IsItemClicked())
			{
				selectedEntity = entity;
			}

		});

	ImGui::End();
}
