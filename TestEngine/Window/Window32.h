#pragma once

#include <windows.h>
#include <windowsx.h>

class Window32
{
public:
	Window32();
	~Window32();

	HWND mHWND = 0;

    bool InitWindow(HINSTANCE instanceHandle, int show);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void Run();

    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nShowCmd)
    {
        // Try to create the window. If it fails exit the program.
        if (!InitWindow(hInstance, nShowCmd))
        {
            return 1; // Return Error and exit program.
        }

        // Begin executing the event and render loop.
        Run();

        return 0;
    }
};

