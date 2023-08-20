#include <SlateEngine/Engine/Input/InputSystem.h>
#include <Windows.h>
#include <windowsx.h>

UINT InputSystem::type = 0;
UINT InputSystem::state = 0;
vec2f InputSystem::pos;
vec2f InputSystem::lastPos;
vec2f InputSystem::delta;

UINT InputSystem::HookMsg(const MSG* msg)
{
    switch (msg->message)
    {
    case WM_LBUTTONDOWN: {
        type = MouseEvent::LPRESS;
    }break;
    case WM_LBUTTONUP: {
        type = MouseEvent::LRELEASE;
    }break;
    case WM_RBUTTONDOWN: {
        type = MouseEvent::RPRESS;
    }break;
    case WM_RBUTTONUP: {
        type = MouseEvent::RRELESE;
    }break;
    case WM_MOUSEMOVE: {
        type = MouseEvent::MOVE;
        pos = { (float)GET_X_LPARAM(msg->lParam), (float)GET_X_LPARAM(msg->lParam) };
        state = (UINT)msg->wParam;
        if (lastPos != pos) {
            delta = pos - lastPos;
        }
        else {
            delta = vec2f();
        }
        lastPos = pos;
    }break;
    default:
        type = MouseEvent::NONE;
        break;
    }
    return 0;  //is not process here 1 is we ownded this event
}

void InputSystem::Update(HWND hwnd) {
    POINT p{};
    GetCursorPos(&p);
    if (hwnd) {
        ScreenToClient(hwnd, &p);
    }
    pos = { (float)p.x, (float)p.y };
    if (lastPos != pos) {
        delta = pos - lastPos;
    }
    else delta = vec2f(0.f);
    lastPos = pos;
}

void InputSystem::DiscardEvents()
{
    type = MouseEvent::NONE;
}

bool InputSystem::IsKeyDown(int keyCode)
{
    return ::GetAsyncKeyState(keyCode) && 0x8000;
}