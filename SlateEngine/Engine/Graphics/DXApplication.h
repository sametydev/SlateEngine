#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Core/Timer.h>
#include <SlateEngine/Engine/Graphics/2D/D2DContext.h>
#include <SlateEngine/Engine/Graphics/DXRasterizerState.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXFrameBuffer.h>
#include <SlateEngine/Engine/Core/ILogger.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXVertexBuffer.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXIndexBuffer.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderCache.h>

#include <SlateEngine/Engine/Core/EnginePlayer.h>

#include <comdef.h>
#include <cstdio>

#define gDXContext DXApplication::Instance->GetDXContext()
#define gDXDevice DXApplication::Instance->GetDXDevice()
#define gDXApp DXApplication::Instance

class ENGINE_API DXApplication
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
    virtual void OnUpdateScene(float deltaTime, ID3D11DeviceContext* pContext)  = 0;
    virtual void OnRenderScene(ID3D11DeviceContext* pContext)                   = 0;
    virtual void OnLateRender()                                                 = 0;
    virtual void OnLateUpdate(float deltaTime)                                  = 0;
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void ClearRenderTarget(float rgba[4]);
    void SwapChainPresent(UINT SyncInterval = 0, UINT Flags = 0);

    inline int GetClientW() { return m_clientW; }
    inline int GetClientH() { return m_clientH; }

    inline ID3D11Device* GetDXDevice() {
        return m_d3dDevice.Get();
    }

    inline ID3D11DeviceContext* GetDXContext() {
        return m_d3dContext.Get();
    }

    inline IDXGISwapChain*     GetDXSwapChain() {
        return m_swapChain.Get();
    }
    inline ID3D11DepthStencilView* GetDepthStencilView() {
        return m_depthStencilView.Get();
    }

    static DXApplication* Instance;


    bool      bEnableMsaa;
    UINT      mMsaaQuality;


    DXGI_ADAPTER_DESC adapterDesc{};

    void SetPlayer(EnginePlayer* player) {
        enginePlayer = player;
    }

    void SetLogger(ILogger* logger) {
        logSystem = logger;
    }

    ILogger* GetLogger() { return logSystem; }

    inline std::string GetWorkingDir() { return this->workingDirectory; };

    inline void SetWorkingDirectory(std::string dir) {
        workingDirectory = dir;
    }

    inline DXFrameBuffer* GetSceneBuffer() { return sceneBuffer; }
    inline DXFrameBuffer* GetOutputBuffer() { return outputBuffer; }

    static float clear[4];

    D3D11_QUERY_DATA_PIPELINE_STATISTICS pipelineStatics = {};


protected:
    bool InitializeWindow();
    bool InitializeGraphics();
    void InitializeHardwareInfo();
    void InitializeFrameBuffers();
    void InitializeDebugLayer();

protected:
    HINSTANCE hInstance;       
    HWND      hWindow;       
    bool      bPaused;      
    bool      bIsMinimized;      
    bool      bIsMaximized;      
    bool      bIsResizing; 

    Timer* mTimer;           

    //DirectX 3D;
    ComPtr<ID3D11Device> m_d3dDevice;                   
    ComPtr<ID3D11DeviceContext> m_d3dContext;  
    ComPtr<IDXGISwapChain> m_swapChain;                 
    //For 11_1
    ComPtr<ID3D11Device1> m_d3dDevice1;                 
    ComPtr<ID3D11DeviceContext1> m_d3dContext1;
    ComPtr<IDXGISwapChain1> m_swapChain1;               

    //RTV and DST
    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;

    D3D11_VIEWPORT m_screenVp{};

    std::wstring sWindowCaption;              
    int m_clientW;                          
    int m_clientH;


    EnginePlayer* enginePlayer = nullptr;
    ILogger* logSystem = nullptr;
    std::string workingDirectory = "";

    //Testing+
    DXFrameBuffer* sceneBuffer = nullptr;
    DXFrameBuffer* outputBuffer = nullptr;

private:
    void CreateOutputBufferResources();
    void RenderOutputBuffer();

    DXVertexBuffer* pOutputVertexBuffer = nullptr;
    DXIndexBuffer* pOutputIndexBuffer = nullptr;
    DXVertexShader* pOutputVertexShader = nullptr;
    DXPixelShader* pOutputPixelShader = nullptr;
};