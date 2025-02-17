#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

struct UIComponent
{
	virtual void Render(ImDrawList* draw_list, ImVec2 pos, ImVec2 av, float size) = 0;
};