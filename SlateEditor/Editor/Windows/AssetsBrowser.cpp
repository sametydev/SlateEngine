#include "AssetsBrowser.h"

AssetsBrowser::AssetsBrowser()
{
}

AssetsBrowser::~AssetsBrowser()
{
}

void AssetsBrowser::OnInit()
{
	windowName = "Asset Browser";
}


void AssetsBrowser::OnDraw()
{
	ImGui::Begin(windowName);

	ImGui::End();
}
