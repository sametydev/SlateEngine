#include <SlateEngine/Engine/Editor/Windows/InspectorWindow.h>
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Editor/Windows/LogWindow.h>
#include <SlateEngine/Engine/Graphics/2D/Components/C2DText.h>
#include <SlateEngine/Engine/Graphics/BuiltInMesh.h>

#include <imgui_internal.h>

InspectorWindow::InspectorWindow()
{
}

InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::OnInit()
{
    windowName = "Inspector";
    game = Game::Instance;
}

void InspectorWindow::OnDraw(const char* title)
{
}

void InspectorWindow::OnDraw()
{
    if (ImGui::Begin("Inspector"))
    {

		Transform& t = game->testEntity->GetComponent<RenderableObject>().GetTransform();

		DrawTransform("Position", t.mPosition);
		DrawTransform("Rotation", t.mRotation);
		DrawTransform("Scale", t.mScale);



        ImGui::Dummy(ImVec2(0.0f, 30.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 70.f;
		ImVec2 buttonSize = { lineHeight + 3.0f, 25 };
		ImGui::Button("Components", buttonSize);
		ImGui::PopStyleColor(3);

		if (ImGui::TreeNode("Material"))
		{
			ImGui::PushID(3);
			ObjectConstantBuffer& objCb = game->testEntity->GetComponent<RenderableObject>().GetObjectCb();
			ImGui::ColorEdit3("Ambient", &objCb.material.ambient.x);
			ImGui::ColorEdit3("Diffuse", &objCb.material.diffuse.x);
			ImGui::ColorEdit3("Specular", &objCb.material.specular.x);
			ImGui::PopID();

			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::Checkbox("WireFrame Mode", &game->renderWireframe);
			ImGui::TreePop();
		}

    }
    ImGui::End();
}

void InspectorWindow::DrawTransform(const char* label, vec3f& val)
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushID(label);
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 80.f);
	ImGui::Dummy(ImVec2(0.0f, 2.0f));
	ImGui::Text(label);
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.5f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::Button("X", buttonSize);
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::DragFloat("##X", &val.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::Button("Y", buttonSize);
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &val.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::Button("Z", buttonSize);
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::DragFloat("##Z", &val.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::PopStyleVar();
	ImGui::Columns(1);
	ImGui::PopID();
}


