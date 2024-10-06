#pragma once
#include <intrin.h>
#include <stdio.h>
#include <string>

struct HWInfo {
    static int cpuinfo[4];
    static std::string gpuName;
    static bool sseSupported;

    static void initialize(const char* _gpuName) {
        __cpuid(cpuinfo, 1);
        //this technically causes memory leak ik but we need this parameter end of the cycle so thats ok
        gpuName = _gpuName;
        sseSupported = cpuinfo[3] & (1 << 25) || false;
    }
};