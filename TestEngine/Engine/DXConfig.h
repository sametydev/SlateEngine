#pragma once
#include <wrl/client.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dxgi1_3.h>
#define _XM_NO_INTRINSICS_

#include <DirectXMath.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"DXGI.lib")

#ifdef _DEBUG
#pragma comment(lib,"dxguid.lib")
#endif

using namespace DirectX;

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

//THIS IS DEPRECATED NEED BE UPDATED
//its very ugly i know but its testing purposes
#define HR_CONSOLE(x)\
{ \
HRESULT hr = (x); \
if(FAILED(hr)) \
{ \
std::cout << "\033[1;31m**************************\n" <<"Error on this file : \n " << __FILE__ << "\n In This Line : " <<(DWORD)__LINE__ << "  HR Code : "<< hr << "\n**************************\n\033[0m\n"<< std::endl; \
} \
}

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
    {															\
        HRESULT hr = (x);										\
        if(FAILED(hr))											\
        {														\
            DXTraceW(__FILEW__, (DWORD)__LINE__, hr, L#x, true);\
        }														\
    }
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif 
#endif

#define SAFE_RELEASE(x) if(x) {x->Release(); x = nullptr;}
#define SAFE_DELETE(x) if(x) {delete x; x = nullptr;}


//For Debugging
inline HRESULT WINAPI DXTraceW(_In_z_ const WCHAR * strFile, _In_ DWORD dwLine, _In_ HRESULT hr,
    _In_opt_ const WCHAR * strMsg, _In_ bool bPopMsgBox)
{
    WCHAR m_bufferFile[MAX_PATH];
    WCHAR m_bufferLine[128];
    WCHAR m_bufferError[300];
    WCHAR m_bufferMsg[1024];
    WCHAR m_bufferHR[40];
    WCHAR m_buffer[3000];

    swprintf_s(m_bufferLine, 128, L"%lu", dwLine);
    if (strFile)
    {
        swprintf_s(m_buffer, 3000, L"%ls(%ls): ", strFile, m_bufferLine);
        OutputDebugStringW(m_buffer);
    }

    size_t nMsgLen = (strMsg) ? wcsnlen_s(strMsg, 1024) : 0;
    if (nMsgLen > 0)
    {
        OutputDebugStringW(strMsg);
        OutputDebugStringW(L" ");
    }
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        m_bufferError, 256, nullptr);

    WCHAR* errorStr = wcsrchr(m_bufferError, L'\r');
    if (errorStr)
    {
        errorStr[0] = L'\0';
    }

    swprintf_s(m_bufferHR, 40, L" (0x%0.8x)", hr);
    wcscat_s(m_bufferError, m_bufferHR);
    swprintf_s(m_buffer, 3000, L"Error Code：%ls", m_bufferError);
    OutputDebugStringW(m_buffer);

    OutputDebugStringW(L"\n");

    if (bPopMsgBox)
    {
        wcscpy_s(m_bufferFile, MAX_PATH, L"");
        if (strFile)
            wcscpy_s(m_bufferFile, MAX_PATH, strFile);

        wcscpy_s(m_bufferMsg, 1024, L"");
        if (nMsgLen > 0)
            swprintf_s(m_bufferMsg, 1024, L"CALL：%ls\n", strMsg);

        swprintf_s(m_buffer, 3000, L"File Name：%ls\nLine：%ls\nError Code：%ls\n%lsDo You Want Debug?",
            m_bufferFile, m_bufferLine, m_bufferError, m_bufferMsg);

        int nResult = MessageBoxW(GetForegroundWindow(), m_buffer, L"BUG", MB_YESNO | MB_ICONERROR);
        if (nResult == IDYES)
            DebugBreak();
    }
    return hr;
}