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

void InspectorWindow::OnDraw()
{
    if (ImGui::Begin("Inspector"))
    {
        ImGui::Text("Object");

        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Text("Material");
        ImGui::PushID(3);
        ImGui::ColorEdit3("Ambient", &game->testEntity->GetComponent<RenderableObject>().GetObjectCb().material.ambient.x);
        ImGui::ColorEdit3("Diffuse", &game->testEntity->GetComponent<RenderableObject>().GetObjectCb().material.diffuse.x);
        ImGui::ColorEdit3("Specular", &game->testEntity->GetComponent<RenderableObject>().GetObjectCb().material.specular.x);
        ImGui::PopID();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Checkbox("WireFrame Mode", &game->renderWireframe);
    }
    ImGui::End();
}
