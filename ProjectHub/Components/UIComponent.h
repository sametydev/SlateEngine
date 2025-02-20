#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern "C"
{
#include <Rpc.h>
}
#pragma comment(lib, "rpcrt4.lib")


struct UIComponent
{
	virtual void Render() = 0;
};