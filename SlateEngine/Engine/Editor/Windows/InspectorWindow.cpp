#include <SlateEngine/Engine/Editor/Windows/InspectorWindow.h>
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Editor/Windows/LogWindow.h>
#include <SlateEngine/Engine/Graphics/2D/Components/C2DText.h>
#include <SlateEngine/Engine/Graphics/BuiltInMesh.h>

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

float ui_x, ui_y;
void InspectorWindow::OnDraw()
{
    if (ImGui::Begin("Inspector"))
    {
        ImGui::Text("Object");
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        static int curr_mesh_item = 0;
        const char* mesh_strs[] = {
            "Box",
            "Sphere",
            "Cylinder",
        };
        if (ImGui::Combo("Set Mesh", &curr_mesh_item, mesh_strs, ARRAYSIZE(mesh_strs)))
        {
            MeshData<VertexPNT> meshData;
            switch (curr_mesh_item)
            {
                case 0: meshData = BuiltInMesh::CreateBox<VertexPNT>(); break;
                case 1: meshData = BuiltInMesh::CreateSphere<VertexPNT>(); break;
                case 2: meshData = BuiltInMesh::CreateCylinder<VertexPNT>(); break;
            }
            game->SetMesh(meshData);
        }

        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Text("Material");
        ImGui::PushID(3);
        ImGui::ColorEdit3("Ambient", &game->m_psCBufferData.material.ambient.x);
        ImGui::ColorEdit3("Diffuse", &game->m_psCBufferData.material.diffuse.x);
        ImGui::ColorEdit3("Specular", &game->m_psCBufferData.material.specular.x);
        ImGui::PopID();

        ImGui::Dummy(ImVec2(0.0f, 23.0f));
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
            game->m_psCBufferData.numSpotLight = 0;
            game->m_psCBufferData.numPointLight = 0;
            game->m_psCBufferData.numDirLight = 1;

            ImGui::ColorEdit3("Ambient", &game->m_psCBufferData.dirLight[0].ambient.x);
            ImGui::ColorEdit3("Diffuse", &game->m_psCBufferData.dirLight[0].diffuse.x);
            ImGui::ColorEdit3("Specular", &game->m_psCBufferData.dirLight[0].specular.x);
        }
        else if (currentLightMode == 1)
        {
            game->m_psCBufferData.numSpotLight = 0;
            game->m_psCBufferData.numPointLight = 1;
            game->m_psCBufferData.numDirLight = 0;

            ImGui::ColorEdit3("Ambient", &game->m_psCBufferData.pointLight[0].ambient.x);
            ImGui::ColorEdit3("Diffuse", &game->m_psCBufferData.pointLight[0].diffuse.x);
            ImGui::ColorEdit3("Specular", &game->m_psCBufferData.pointLight[0].specular.x);
            ImGui::InputFloat("Range", &game->m_psCBufferData.pointLight[0].range);
            ImGui::InputFloat3("Attenutation", &game->m_psCBufferData.pointLight[0].attenutation.x);
        }
        else
        {
            game->m_psCBufferData.numSpotLight = 1;
            game->m_psCBufferData.numPointLight = 0;
            game->m_psCBufferData.numDirLight = 0;

            ImGui::ColorEdit3("Ambient", &game->m_psCBufferData.spotLight[0].ambient.x);
            ImGui::ColorEdit3("Diffuse", &game->m_psCBufferData.spotLight[0].diffuse.x);
            ImGui::ColorEdit3("Specular", &game->m_psCBufferData.spotLight[0].specular.x);
            ImGui::InputFloat("Spot", &game->m_psCBufferData.spotLight[0].spot);
            ImGui::InputFloat("Range", &game->m_psCBufferData.spotLight[0].range);
            ImGui::InputFloat3("Attenutation", &game->m_psCBufferData.spotLight[0].attenutation.x);
        }
        ImGui::PopID();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Checkbox("WireFrame Mode", &game->renderWireframe);

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::Text("UI Text Summoner:");
        
        ImGui::InputFloat("Text X", &ui_x);
        ImGui::InputFloat("Text Y", &ui_y);
        static char bufpass[64] = "MyLabel";
        ImGui::InputText("Label", bufpass,64);
        if (ImGui::Button("Add Text To Render"))
        {
            new C2DText(charToWChar(bufpass), ui_x, ui_y, 600.0f, 200.0f);
        }
    }
    ImGui::End();
}