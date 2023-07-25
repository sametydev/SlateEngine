#include <TestEngine/Engine/Editor/Windows/InspectorWindow.h>
#include <TestEngine/Engine/DXConfig.h>
#include <TestEngine/Engine/Graphics/DXApplication.h>
#include <TestEngine/Engine/Editor/Windows/LogWindow.h>
#include <TestEngine/Engine/Graphics/2D/Components/C2DText.h>
#include <TestEngine/Engine/Graphics/BuiltInMesh.h>

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
            MeshData<VertexPNC> meshData;
            switch (curr_mesh_item)
            {
            case 0: meshData = BuiltInMesh::CreateBox<VertexPNC>(); break;
            case 1: meshData = BuiltInMesh::CreateSphere<VertexPNC>(); break;
            case 2: meshData = BuiltInMesh::CreateCylinder<VertexPNC>(); break;
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
        static int currentLightMode = 0;
        static const char* lightModes[] = {
            "Directional Light",
            "Point Light",
            "Spot Light"
        };
        ImGui::Text("Light");
        if (ImGui::Combo("Light Type", &currentLightMode, lightModes, ARRAYSIZE(lightModes)))
        {
            game->m_psCBufferData.dirLight = (currentLightMode == 0 ? game->m_directionalLight : DirectionalLight());
            game->m_psCBufferData.pointLight = (currentLightMode == 1 ? game->m_pointLight : PointLight());
            game->m_psCBufferData.spotLight = (currentLightMode == 2 ? game->m_spotLight : SpotLight());
        }

        bool light_changed = false;

        ImGui::PushID(currentLightMode);
        if (currentLightMode == 0)
        {
            ImGui::ColorEdit3("Ambient", &game->m_psCBufferData.dirLight.ambient.x);
            ImGui::ColorEdit3("Diffuse", &game->m_psCBufferData.dirLight.diffuse.x);
            ImGui::ColorEdit3("Specular", &game->m_psCBufferData.dirLight.specular.x);
        }
        else if (currentLightMode == 1)
        {
            ImGui::ColorEdit3("Ambient", &game->m_psCBufferData.pointLight.ambient.x);
            ImGui::ColorEdit3("Diffuse", &game->m_psCBufferData.pointLight.diffuse.x);
            ImGui::ColorEdit3("Specular", &game->m_psCBufferData.pointLight.specular.x);
            ImGui::InputFloat("Range", &game->m_psCBufferData.pointLight.range);
            ImGui::InputFloat3("Attenutation", &game->m_psCBufferData.pointLight.attenutation.x);
        }
        else
        {
            ImGui::ColorEdit3("Ambient", &game->m_psCBufferData.spotLight.ambient.x);
            ImGui::ColorEdit3("Diffuse", &game->m_psCBufferData.spotLight.diffuse.x);
            ImGui::ColorEdit3("Specular", &game->m_psCBufferData.spotLight.specular.x);
            ImGui::InputFloat("Spot", &game->m_psCBufferData.spotLight.spot);
            ImGui::InputFloat("Range", &game->m_psCBufferData.spotLight.range);
            ImGui::InputFloat3("Attenutation", &game->m_psCBufferData.spotLight.attenutation.x);
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
