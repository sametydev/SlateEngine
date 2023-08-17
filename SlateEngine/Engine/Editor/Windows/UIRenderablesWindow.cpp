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
		new C2DText(charToWChar(bufpass), ui_x, ui_y, 600.0f, 200.0f);
	}

	for (auto r : D2DContext::Instance->m_texts)
	{
		if (ImGui::TreeNode(r->GetText()))
		{
			ImGui::TreePop();
		}
	}

	ImGui::End();
}
