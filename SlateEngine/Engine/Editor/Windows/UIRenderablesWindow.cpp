#include <SlateEngine/Engine/Editor/Windows/UIRenderablesWindow.h>

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


void UIRenderablesWindow::OnDraw(const char* title)
{

}

float ui_x, ui_y;
void UIRenderablesWindow::OnDraw()
{
	ImGui::Begin(windowName);

	ImGui::Dummy(ImVec2(0.0f, 15.0f));
	ImGui::Text("UI Text Summoner:");

	ImGui::InputFloat("Text X", &ui_x);
	ImGui::InputFloat("Text Y", &ui_y);
	static char bufpass[64] = "MyLabel";
	ImGui::InputText("Label", bufpass, 64);
	if (ImGui::Button("Add Text To Render"))
	{
		D2DContext::Instance->AddTextForRender(charToWChar(bufpass), ui_x, ui_y, 600.0f, 200.0f);
	}

	D2DContext::Instance->GetRegistar().each([](auto entity)
		{
			C2DText& name = D2DContext::Instance->GetRegistar().get<C2DText>(entity);
			if (ImGui::TreeNode(name.GetText()))
			{
				ImGui::TreePop();
			}
		});

	ImGui::End();
}
