#include "HWInfo.h"

int HWInfo::cpuinfo[4] = { 0 };
std::string HWInfo::gpuName = "";
const char* HWInfo::gpuName_cstr = nullptr;
bool HWInfo::sseSupported = false;

void HWInfo::initialize(const char* _gpuName) {
    __cpuid(cpuinfo, 1);
    gpuName = _gpuName;
    gpuName_cstr = gpuName.c_str();
    sseSupported = cpuinfo[3] & (1 << 25) || false;
}