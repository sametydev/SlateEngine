#pragma once
#include "UIComponent.h"
#include "ComponentLookup.h"
#include <string>
struct CProjectItemUI :
    public UIComponent
{
public:
    std::string projectName;
    CProjectItemUI();
    ~CProjectItemUI();
    void Render(ImDrawList* draw_list, ImVec2 pos, ImVec2 av, float size) override;

private:
    int objectID = 0;
    const float damping = 50.f;
};

