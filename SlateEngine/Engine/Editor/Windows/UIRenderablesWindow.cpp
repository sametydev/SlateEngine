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

void UIRenderablesWindow::OnDraw()
{
	ImGui::Begin(windowName);

	for (auto r : D2DContext::Instance->m_texts)
	{
		if (ImGui::TreeNode(r->GetText()))
		{
			ImGui::TreePop();
		}
	}

	ImGui::End();
}
