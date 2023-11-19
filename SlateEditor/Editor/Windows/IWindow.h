#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

class IWindow {
public:
	IWindow(){}
	virtual ~IWindow(){}

	virtual void OnInit() = 0;
	virtual void OnDraw() = 0;

private:
	const char* windowName;
};