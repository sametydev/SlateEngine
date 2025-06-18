#include "ProfilerWindow.h"
#include <Engine\Core\ProfileTimerFactory.h>

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
		ImGui::Text("You can see profile results and engine functions call time/s");
		for (auto& result : ProfileTimerFactory::Instance->GetResults()) {
			char buffer[50];
			strcpy_s(buffer, result.first.c_str());
			strcat_s(buffer, " %.2fms");
			ImGui::Text(buffer, result.second);
		}
	}
}
