#include "ToolboxWindow.h"
#include <ImGuizmo.h>

ToolboxWindow::ToolboxWindow()
{
}

ToolboxWindow::~ToolboxWindow()
{
}

void ToolboxWindow::OnInit()
{
	game = Game::Instance;
	windowName = "Tools";
}

void ToolboxWindow::OnDraw(bool* closable)
{
	ImGui::Begin(windowName);
	if (ImGui::Button("PLAY")) { game->SetGameState((GameState)1); }
	ImGui::SameLine();
	if (ImGui::Button("PAUSE")) { game->SetGameState((GameState)2); }
	ImGui::SameLine();
	if (ImGui::Button("STOP")) { game->SetGameState((GameState)0); }

	ImGui::SameLine();
	ImGui::Dummy({ 20.f,20.f });
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	if (ImGui::Button("T")) { gizmoType = ImGuizmo::OPERATION::TRANSLATE; }
	ImGui::PopStyleColor(1);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0.71f, 0.0f, 1.0f });
	if (ImGui::Button("R")) { gizmoType = ImGuizmo::OPERATION::ROTATE; }
	ImGui::PopStyleColor(1);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0.0f, 1.0f, 1.0f });
	if (ImGui::Button("S")) { gizmoType = ImGuizmo::OPERATION::SCALE; }
	ImGui::PopStyleColor(1);

	ImGui::Dummy({ 175.f,20.f });
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0.3f, 0.3f, 0.3f });
	if (ImGui::Button("U")) { gizmoType = ImGuizmo::OPERATION::UNIVERSAL; }
	ImGui::PopStyleColor(1);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.5f, 0.3f, 0.9f, 1.0f });
	if (ImGui::Button("RS")) { gizmoType = ImGuizmo::OPERATION::ROTATE_SCREEN; }
	ImGui::PopStyleColor(1);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.8f, 0.25f, 1.0f });
	if (ImGui::Button("SU")) { gizmoType = ImGuizmo::OPERATION::SCALEU; }
	ImGui::PopStyleColor(1);

	ImGui::End();
}
