#include "ComponentLookup.h"
std::unordered_map<ComponentID, int> ComponentLookup::globalCounter = {
	{0,-1}
};
std::unordered_map<std::string, int> ComponentLookup::componentLookupTable = {
	{typeid(CProjectItemUI).name(),0}
};