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
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			//Pause / Stop
		}
		else
		{
			// Continue
		}
		return 0;
	default:
		break;
	}

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
	return static_cast<float> (m_clientw) / m_clienth;
}

bool DXApplication::InitializeWindow()
{
	return false;
}

bool DXApplication::InitializeGraphics()
{
	return false;
}
