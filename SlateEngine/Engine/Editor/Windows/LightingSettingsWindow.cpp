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

void LightingSettingsWindow::OnDraw(const char* title)
{
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
        }
        ImGui::PopID();
    }
    ImGui::End();
}
