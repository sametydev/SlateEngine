#include "ProfilerWindow.h"
#include <Engine\Core\ProfileTimerFactory.h>
#include <format>

ProfilerWindow::ProfilerWindow()
{
}

ProfilerWindow::~ProfilerWindow()
{
}

void ProfilerWindow::OnInit()
{
}

void ProfilerWindow::OnDraw(bool* closable)
{
	if (*closable) {
		if (ImGui::Begin("Profiler", closable)) {

			ImGui::Text("You can see profile results and engine functions call time/s");
			char buffer[128];
			for (auto& result : ProfileTimerFactory::Instance->GetResults()) {
				/* THIS IS FOR DYNAMIC IMPLEMENTATION, ITS SAFE, BUT SO MANY ALLOCATION AND FREE CALLS
				size_t sizeofstr = result.first.size() + strlen(" %.2fms") + 1;
				char* buffer = (char*)malloc(sizeofstr);
				strcpy_s(buffer, sizeofstr, result.first.c_str());
				strcat_s(buffer, sizeofstr, " %.2fms");
				*/
				strcpy_s(buffer, sizeof(buffer), result.first.c_str());
				strcat_s(buffer, sizeof(buffer), " %.2fms");
				ImGui::Text(buffer, result.second);
				//free(buffer);
			}
		}ImGui::End();
	}
}
