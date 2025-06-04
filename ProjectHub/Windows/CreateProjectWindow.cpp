#include "CreateProjectWindow.h"

CreateProjectWindow::CreateProjectWindow()
{
	p_closable = new bool;
	*p_closable = false;
}

CreateProjectWindow::~CreateProjectWindow()
{
	delete p_closable;
}

void CreateProjectWindow::Render()
{
	if (*p_closable)
	{
		if (ImGui::Begin("Create Project", p_closable)) {

		}ImGui::End();
	}
}

void CreateProjectWindow::SetOpen(bool val)
{
	*p_closable = val;
}
