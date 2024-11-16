#include "LightingSettingsWindow.h"

LightingSettingsWindow::LightingSettingsWindow()
{
}

LightingSettingsWindow::~LightingSettingsWindow()
{
}

void LightingSettingsWindow::OnInit()
{
    windowName = "Lightning";
    game = Game::Instance;
}

void LightingSettingsWindow::DrawTransform(const char* label, vec3f& val)
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

void LightingSettingsWindow::OnDraw()
{
    if (ImGui::Begin("Light"))
    {
        static int currentLightMode = 1;
        static const char* lightModes[] = {
            "Directional Light",
            "Point Light",
            "Spot Light"
        };
        ImGui::Text("Light");
        ImGui::Combo("Light Type", &currentLightMode, lightModes, ARRAYSIZE(lightModes));

        bool light_changed = false;

        ImGui::PushID(currentLightMode);
        if (currentLightMode == 0)
        {
            game->LightConstantObject.numSpotLight = 0;
            game->LightConstantObject.numPointLight = 0;
            game->LightConstantObject.numDirLight = 1;

            ImGui::ColorEdit3("Ambient", &game->LightConstantObject.dirLight[0].ambient.x);
            ImGui::ColorEdit3("Diffuse", &game->LightConstantObject.dirLight[0].diffuse.x);
            ImGui::ColorEdit3("Specular", &game->LightConstantObject.dirLight[0].specular.x);
            DrawTransform("Direction", game->LightConstantObject.dirLight[0].direction);
        }
        else if (currentLightMode == 1)
        {
            game->LightConstantObject.numSpotLight = 0;
            game->LightConstantObject.numPointLight = 1;
            game->LightConstantObject.numDirLight = 0;

            ImGui::ColorEdit3("Ambient", &game->LightConstantObject.pointLight[0].ambient.x);
            ImGui::ColorEdit3("Diffuse", &game->LightConstantObject.pointLight[0].diffuse.x);
            ImGui::ColorEdit3("Specular", &game->LightConstantObject.pointLight[0].specular.x);
            ImGui::InputFloat("Range", &game->LightConstantObject.pointLight[0].range);
            ImGui::InputFloat3("Attenutation", &game->LightConstantObject.pointLight[0].attenutation.x);
            DrawTransform("Position", game->LightConstantObject.pointLight[0].position);
        }
        else
        {
            game->LightConstantObject.numSpotLight = 1;
            game->LightConstantObject.numPointLight = 0;
            game->LightConstantObject.numDirLight = 0;

            ImGui::ColorEdit3("Ambient", &game->LightConstantObject.spotLight[0].ambient.x);
            ImGui::ColorEdit3("Diffuse", &game->LightConstantObject.spotLight[0].diffuse.x);
            ImGui::ColorEdit3("Specular", &game->LightConstantObject.spotLight[0].specular.x);
            ImGui::InputFloat("Spot", &game->LightConstantObject.spotLight[0].spot);
            ImGui::InputFloat("Range", &game->LightConstantObject.spotLight[0].range);
            ImGui::InputFloat3("Attenutation", &game->LightConstantObject.spotLight[0].attenutation.x);
            DrawTransform("Position", game->LightConstantObject.spotLight[0].position);
        }
        ImGui::PopID();
    }
    ImGui::End();
}
