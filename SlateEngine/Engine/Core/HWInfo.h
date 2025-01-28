#pragma once
#include <intrin.h>
#include <stdio.h>
#include <string>

#include <SlateEngine/Engine/Utils.h>

struct __declspec(dllexport) HWInfo  {
    static int cpuinfo[4];
    static std::string gpuName;
    static const char* gpuName_cstr;
    static bool sseSupported;

    static void initialize(std::string _gpuName);
};