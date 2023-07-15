#include <TestEngine/Engine/Application/DXApplication.h>

DXApplication::DXApplication(HINSTANCE hInstance)
{
}

DXApplication::~DXApplication()
{
}

void DXApplication::OnRun()
{
}

bool DXApplication::OnInit()
{
	return false;
}

void DXApplication::OnResize()
{
}

void DXApplication::OnUpdateScene(float dt)
{
}

void DXApplication::OnRenderScene()
{
}

LRESULT DXApplication::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

HINSTANCE DXApplication::GetApplicationInstance() const
{
	return HINSTANCE();
}

HWND DXApplication::GetMainHWND() const
{
	return HWND();
}

float DXApplication::GetAspectRatio() const
{
	return 0.0f;
}

bool DXApplication::InitializeWindow()
{
	return false;
}

bool DXApplication::InitializeGraphics()
{
	return false;
}
