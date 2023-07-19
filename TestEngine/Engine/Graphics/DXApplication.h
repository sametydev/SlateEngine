#pragma once

#include <TestEngine/Engine/DXConfig.h>
#include <TestEngine/Engine/Core/Timer.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

class DXApplication
{
public:

    DXApplication(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    virtual ~DXApplication();

    HINSTANCE ProcInstance()        const;                   
    HWND      MainWnd()             const;                   
    float     GetAspectRatio()      const;               
    int OnRun();                                  
    virtual bool OnInit();                        
    virtual void OnResize();                   
    virtual void OnUpdateScene(float deltaTime) = 0;     
    virtual void OnRenderScene()         = 0;               
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



    inline ComPtr<ID3D11Device>& GetDXDevice() {
        return m_d3dDevice;
    }

    inline ComPtr<ID3D11DeviceContext>& GetDXContext() {
        return m_d3dContext;
    }

    inline ComPtr<IDXGISwapChain>&     GetDXSwapChain() {
        return m_swapChain;
    }

    static DXApplication* Instance;

protected:
    bool InitializeWindow();      
    bool InitializeGraphics();        

protected:

    HINSTANCE hInstance;       
    HWND      hWindow;       
    bool      bPaused;      
    bool      bIsMinimized;      
    bool      bIsMaximized;      
    bool      bIsResizing;       
    bool      bEnableMsaa;   
    UINT      mMsaaQuality;  


    Timer mTimer;           

    ComPtr<ID3D11Device> m_d3dDevice;                   
    ComPtr<ID3D11DeviceContext> m_d3dContext;  
    ComPtr<IDXGISwapChain> m_swapChain;                 

    ComPtr<ID3D11Device1> m_d3dDevice1;                 
    ComPtr<ID3D11DeviceContext1> m_d3dContext1;
    ComPtr<IDXGISwapChain1> m_swapChain1;               

    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;       
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;  
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;  
    D3D11_VIEWPORT m_screenVp;                     

    std::wstring sWindowCaption;                       
    int m_clientW;                                   
    int m_clientH;

};
namespace
{
    //static DXApplication* DXApp = nullptr;
}