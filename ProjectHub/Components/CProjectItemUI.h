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
    void Render() override;

    inline std::string& GetUUID() {
        return mUUID;
    };

private:
    int objectID = 0;
    const float damping = 50.f;
    ImColor color = ImColor(38, 44, 54);
    std::string mUUID;

};

