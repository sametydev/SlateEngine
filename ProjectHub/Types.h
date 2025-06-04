#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <functional>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern "C"
{
#include <Rpc.h>
}
#pragma comment(lib, "rpcrt4.lib")


struct Project {
	std::string projectName;
	std::string projectPath;
	std::string projectTemplate;
	unsigned int engineVersion = 0;
};