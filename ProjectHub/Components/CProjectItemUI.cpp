#include "CProjectItemUI.h"
#include <Windows.h>
#include <cstdlib> 
#include <ctime> 
#include <iostream>

CProjectItemUI::CProjectItemUI()
{
    unsigned int compID = ComponentLookup::componentLookupTable[typeid(this).name()];
    objectID = (ComponentLookup::globalCounter[compID] = ComponentLookup::globalCounter[compID] + 1);

    UUID uuid;
    UuidCreate(&uuid);

    unsigned char* str;
    UuidToStringA(&uuid, &str);

    mUUID = ((char*)str);

    RpcStringFreeA(&str);
}


void CProjectItemUI::Render()
{
    const ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    const float itemWidth = ImGui::GetContentRegionAvail().x;
    const float itemHeight = 40.0f;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(cursorPos,
        { cursorPos.x + itemWidth, cursorPos.y + itemHeight },
        IM_COL32(50, 50, 50, 255));

    ImGui::SetCursorScreenPos({ cursorPos.x + 8, cursorPos.y+3 });
    ImGui::Text("Test Project");

    ImGui::SetCursorScreenPos({ cursorPos.x + 8, cursorPos.y + 20 });
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 125));
    ImGui::Text("C:/TestUser/BlaBla/Test Project");
    ImGui::PopStyleColor();

    ImGui::SetCursorScreenPos({ cursorPos.x + itemWidth - 58, cursorPos.y + 5 });
    ImGui::PushID(mUUID.c_str());
    if (ImGui::Button("OPEN", {50, 30})) {
        MessageBox(0, L"Coming Soon", L"Coming Soon", 0);
    }
    ImGui::PopID();
    ImGui::SetCursorScreenPos({ cursorPos.x, cursorPos.y + itemHeight + 5 });
}


CProjectItemUI::~CProjectItemUI()
{
    unsigned int compID = ComponentLookup::componentLookupTable[typeid(this).name()];
    ComponentLookup::globalCounter[compID] = ComponentLookup::globalCounter[compID] - 1;
}