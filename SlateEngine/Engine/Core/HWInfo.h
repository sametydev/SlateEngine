#pragma once
#include <intrin.h>
#include <stdio.h>
#include <string>

struct HWInfo {
    static int cpuinfo[4];
    static std::string gpuName;
    static const char* gpuName_cstr;
    static bool sseSupported;

    static void initialize(const char* _gpuName) {
        __cpuid(cpuinfo, 1);
        gpuName = _gpuName;
        //ik so bad
        gpuName_cstr = gpuName.c_str();
        sseSupported = cpuinfo[3] & (1 << 25) || false;
    }
};