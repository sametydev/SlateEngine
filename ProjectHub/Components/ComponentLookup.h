#pragma once
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <string>
#include "CProjectItemUI.h"
using ComponentID = int;

class ComponentLookup {
public:
	static std::unordered_map<std::string, int> componentLookupTable;
	static std::unordered_map<ComponentID, int> globalCounter;
};
