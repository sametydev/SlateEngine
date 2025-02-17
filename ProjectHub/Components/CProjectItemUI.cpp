#include "CProjectItemUI.h"

CProjectItemUI::CProjectItemUI()
{
    int compID = ComponentLookup::componentLookupTable[typeid(this).name()];
    objectID = (ComponentLookup::globalCounter[compID] = ComponentLookup::globalCounter[compID] + 1);
}



void CProjectItemUI::Render(ImDrawList* draw_list,ImVec2 pos,ImVec2 av, float size)
{
    float x = pos.x + 4.0f, y = pos.y + 4.0f;
    ImColor color = ImColor(38, 44, 54);
    draw_list->AddRectFilled(ImVec2(x, y + (objectID * damping)), ImVec2(av.x, (y + size) + (objectID * damping)), color);
    draw_list->AddText(ImVec2(x, y + (objectID * damping)), 0xffffffff, "Test Project");
    ImGui::SetCursorPos({ av.x - 50.0f,y + (objectID * damping)});
    ImGui::Button("OPEN", { 50,size });
}

CProjectItemUI::~CProjectItemUI()
{
    int compID = ComponentLookup::componentLookupTable[typeid(this).name()];
    ComponentLookup::globalCounter[compID] = ComponentLookup::globalCounter[compID] - 1;
}