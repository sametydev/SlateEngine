#include <TestEngine/Window/Window32.h>

Window32::Window32()
{
}

Window32::~Window32()
{
}

bool Window32::InitWindow(HINSTANCE instanceHandle, int show)
{
    WNDCLASS wc{}; // Datastructure that holds the details of the windowclass which describes our window.

    wc.style = CS_HREDRAW | CS_VREDRAW;                  // Class styling. Allows for additional behaviours of the window.
    wc.lpfnWndProc = WndProc;                                  // A function pointer to the Window Procedure.
    wc.cbClsExtra = 0;                                        // Extra bytes to allocate to the window class structure.
    wc.cbWndExtra = 0;                                        // Extra bytes to allocate to the window instance.
    wc.hInstance = instanceHandle;                           // The module handle of this application.
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);           // Icon of the window.
    wc.hCursor = LoadCursor(0, IDC_ARROW);               // Cursor used by the window.
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // Paints the window white.
    wc.lpszMenuName = 0;                                        // Name of an associated menu.
    wc.lpszClassName = L"D3DWindowClass";                        // Name of the window class this structure will become
}

LRESULT Window32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_LBUTTONDOWN: // Left Mousclick
        MessageBox(0, L"Hello, World", L"Hello", MB_OK);
        return 0;
    case WM_KEYDOWN: // keypress (down position)
        if (wParam == VK_ESCAPE) // Escape Key
        {
            DestroyWindow(hWnd);
        }
        return 0;
    case WM_DESTROY: // Window was closed by us or by the user
        DestroyWindow(hWnd);
        mHWND = 0;
        PostQuitMessage(0); // Send the quit message
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam); // Send messages back to the OS.
        break;
    }
}

void Window32::Run()
{
}
