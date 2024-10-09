#include "HWInfo.h"

int HWInfo::cpuinfo[4] = { 0 };
std::string HWInfo::gpuName = "null";
bool HWInfo::sseSupported = false;
const char* HWInfo::gpuName_cstr = nullptr;