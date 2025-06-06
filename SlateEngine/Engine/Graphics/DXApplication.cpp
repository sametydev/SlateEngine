﻿#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Core/EngineConfig.h>
#include <sstream>
#include <SlateEngine/Engine/Input/InputSystem.h>
#include <SlateEngine/Engine/Graphics/DXBasicBatch.h>
#include <codecvt>

#pragma warning(disable: 6031)

extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x00000001;
}


DXApplication* DXApplication::Instance = nullptr;
float DXApplication::clear[4] = { 0.05f, 0.05f, 0.05f, 1.0f };

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return DXApplication::Instance->MsgProc(hwnd, msg, wParam, lParam);
}

DXApplication::DXApplication(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : hInstance(hInstance),
    sWindowCaption(windowName),
    m_clientW(initWidth),
    m_clientH(initHeight),
    hWindow(nullptr),
    bPaused(false),
    bIsMinimized(false),
    bIsMaximized(false),
    bIsResizing(false),
    bEnableMsaa(true),
    mMsaaQuality(0),
    m_d3dDevice(nullptr),
    m_d3dContext(nullptr),
    m_swapChain(nullptr),
    m_depthStencilBuffer(nullptr),
    m_renderTargetView(nullptr),
    m_depthStencilView(nullptr)
{

    if (!Instance)
    {
        mTimer = new Timer();
        Instance = this;
    }
}

DXApplication::~DXApplication()
{
    if (m_d3dContext)
        m_d3dContext->ClearState();
}

HINSTANCE DXApplication::ProcInstance()const
{
    return hInstance;
}

HWND DXApplication::MainWnd()const
{
    return hWindow;
}

float DXApplication::GetAspectRatio()const
{
    return static_cast<float>(m_clientW) / static_cast<float>(m_clientH);
}

int DXApplication::OnRun()
{
    MSG msg = { 0 };

    mTimer->OnReset();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            mTimer->OnTick();
            InputSystem::Update(hWindow);
            if (!bPaused)
            {
                //Frame buffer 1 (drawing operation)
                sceneBuffer->BeginFrameById(0);
                sceneBuffer->Clear(clear);
                OnUpdateScene(mTimer->deltaTime(),m_d3dContext.Get());
                OnRenderScene(m_d3dContext.Get());
                enginePlayer->OnRenderScene(m_d3dContext.Get());
                sceneBuffer->EndFrame(m_renderTargetView.Get(),m_depthStencilView.Get());

                //Frame buffer 2 (drawing framebuffer 1 output as a fullscreen quad)
                outputBuffer->BeginFrameById(0);
                outputBuffer->Clear(clear);
                RenderOutputBuffer();
                outputBuffer->EndFrame(m_renderTargetView.Get(), m_depthStencilView.Get());

                ClearRenderTarget(clear);

                //Render Player
                enginePlayer->NewFrame();
                enginePlayer->ClearViewport(clear);
                enginePlayer->OnUpdate(mTimer->deltaTime());
                enginePlayer->OnRender(clear);


                OnLateUpdate(mTimer->deltaTime());
                OnLateRender();

                SwapChainPresent();
            }
            else
            {
                Sleep(100);
            }
        }
    }

    return (int)msg.wParam;
}

bool DXApplication::OnInit()
{
    if (!InitializeWindow())
        return false;

    if (!InitializeGraphics())
        return false;

    DXRasterizerState::Initialize(GetDXDevice(), GetDXContext(), bEnableMsaa);
    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_d3dContext->Flush();

    CreateOutputBufferResources();
    return true;
}

void DXApplication::OnResize()
{
    FrameBufferDesc desc{};
    desc.bDepthStencil = true;
    desc.height = m_clientH;
    desc.width = m_clientW;
    desc.nRenderPass = 1;
    sceneBuffer->Create(desc);
    outputBuffer->Create(desc);

    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_depthStencilBuffer.Reset();

    ComPtr<ID3D11Texture2D> backBuffer;
    HR(m_swapChain->ResizeBuffers(1, m_clientW, m_clientH, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
    HR(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf()));
    backBuffer.Reset();


    D3D11_TEXTURE2D_DESC depthStencilDesc{};

    depthStencilDesc.Width = m_clientW;
    depthStencilDesc.Height = m_clientH;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    if (bEnableMsaa)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = mMsaaQuality - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    HR(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_depthStencilBuffer.GetAddressOf()));
    HR(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, m_depthStencilView.GetAddressOf()));


    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    m_screenVp.TopLeftX = 0;
    m_screenVp.TopLeftY = 0;
    m_screenVp.Width = static_cast<float>(m_clientW);
    m_screenVp.Height = static_cast<float>(m_clientH);
    m_screenVp.MinDepth = 0.0f;
    m_screenVp.MaxDepth = 1.0f;

    m_d3dContext->RSSetViewports(1, &m_screenVp);
}

LRESULT DXApplication::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
        {
            bPaused = true;
            //m_Timer.Stop();
        }
        else
        {
            bPaused = false;
            //m_Timer.Start();
        }
        return 0;
    case WM_SIZE:
        m_clientW = LOWORD(lParam);
        m_clientH = HIWORD(lParam);
        if (m_d3dDevice)
        {
            if (wParam == SIZE_MINIMIZED)
            {
                bPaused = true;
                bIsMinimized = true;
                bIsMaximized = false;
            }
            else if (wParam == SIZE_MAXIMIZED)
            {
                bPaused = false;
                bIsMinimized = false;
                bIsMaximized = true;
                OnResize();
            }
            else if (wParam == SIZE_RESTORED)
            {
                if (bIsMinimized)
                {
                    bPaused = false;
                    bIsMinimized = false;
                    OnResize();
                }
                else if (bIsMaximized)
                {
                    bPaused = false;
                    bIsMaximized = false;
                    OnResize();
                }
                else
                {
                    OnResize();
                }
            }
        }
        return 0;
    case WM_ENTERSIZEMOVE:
        bPaused = true;
        bIsResizing = true;
        //m_Timer.Stop();
        return 0;
    case WM_EXITSIZEMOVE:
        bPaused = false;
        bIsResizing = false;
        //m_Timer.Start();
        OnResize();
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_MENUCHAR:
        return MAKELRESULT(0, MNC_CLOSE);
    case WM_GETMINMAXINFO:
        ((MINMAXINFO*)lParam)->ptMinTrackSize.x = MIN_WIN_HEIGHT;
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = MIN_WIN_WIDTH;
        return 0;
    }
    enginePlayer->MsgProc(hwnd, msg, wParam, lParam);
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void DXApplication::ClearRenderTarget(float rgba[4])
{
    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), rgba);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DXApplication::SwapChainPresent(UINT SyncInterval, UINT Flags)
{
    m_swapChain->Present(0, 0);
}


bool DXApplication::InitializeWindow()
{
    WNDCLASS wc{ 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszClassName = L"SlateEngineWND";

    if (!RegisterClass(&wc))
    {
        MessageBox(0, L"RegisterClass Failed.", 0, 0);
        return false;
    }

    // Compute window rectangle dimensions based on requested client area dimensions.
    RECT R = { 0, 0, m_clientW, m_clientH };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
    int width = R.right - R.left;
    int height = R.bottom - R.top;

    hWindow = CreateWindow(L"SlateEngineWND", sWindowCaption.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);

    if (!hWindow)
    {
        MessageBox(0, L"CreateWindow Failed.", 0, 0);
        return false;
    }

    ShowWindow(hWindow, SW_SHOW);
    UpdateWindow(hWindow);

    return true;
}

bool DXApplication::InitializeGraphics()
{

    HRESULT hr = S_OK;

    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    D3D_FEATURE_LEVEL featureLevel;
    D3D_DRIVER_TYPE d3dDriverType;
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        d3dDriverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &featureLevel, m_d3dContext.GetAddressOf());

        if (hr == E_INVALIDARG)
        {
            hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &featureLevel, m_d3dContext.GetAddressOf());
        }

        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
    {
        MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
        return false;
    }
    if (featureLevel != D3D_FEATURE_LEVEL_11_1)
    {
        MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
        return false;
    }

    m_d3dDevice->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM, 4, &mMsaaQuality);
    assert(mMsaaQuality > 0);


    ComPtr<IDXGIDevice> dxgiDevice     = nullptr;
    ComPtr<IDXGIAdapter> dxgiAdapter   = nullptr;
    ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;
    ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;

    HR(m_d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
    HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
    HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf())));

    // Check supports of DX 11.1
    HR(dxgiFactory1->QueryInterface(IID_PPV_ARGS(&dxgiFactory2)));

    HR(m_d3dDevice->QueryInterface(IID_PPV_ARGS(& m_d3dDevice1)));
    HR(m_d3dContext->QueryInterface(IID_PPV_ARGS(& m_d3dContext1)));

    DXGI_SWAP_CHAIN_DESC1 sd{};
    sd.Width                           = m_clientW;
    sd.Height                          = m_clientH;
    sd.Format                          = DXGI_FORMAT_R8G8B8A8_UNORM;

    if (bEnableMsaa)
    {
        sd.SampleDesc.Count            = 4;
        sd.SampleDesc.Quality          = mMsaaQuality - 1;
    }
    else
    {
        sd.SampleDesc.Count            = 1;
        sd.SampleDesc.Quality          = 0;
    }
    sd.BufferUsage                     = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount                     = 1;
    sd.SwapEffect                      = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags                           = 0;
    sd.Scaling                         = DXGI_SCALING_STRETCH;
    sd.AlphaMode                       = DXGI_ALPHA_MODE_UNSPECIFIED;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd{};
    fd.RefreshRate.Numerator           = MAX_FPS;
    fd.RefreshRate.Denominator         = 1;
    fd.Scaling                         = DXGI_MODE_SCALING_UNSPECIFIED;
    fd.ScanlineOrdering                = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    fd.Windowed                        = TRUE;

    HR(dxgiFactory2->CreateSwapChainForHwnd(m_d3dDevice.Get(), hWindow, &sd, &fd, nullptr, m_swapChain1.GetAddressOf()));
    HR(m_swapChain1->QueryInterface(IID_PPV_ARGS(&m_swapChain)));

    dxgiFactory1->MakeWindowAssociation(hWindow, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);
    dxgiAdapter->GetDesc(&adapterDesc);


    //Needs refactoring here;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //new DXBasicBatch(m_d3dDevice.Get(), m_d3dContext.Get());
    //DXBasicBatch::Instance->Init();
    ////////////////////////////////////////////////////////////////////////////////////

    InitializeHardwareInfo();
    InitializeFrameBuffers();
    InitializeDebugLayer();

    return true;
}

void DXApplication::InitializeHardwareInfo()
{
    std::wstring __desc_w(adapterDesc.Description);
    HWInfo::initialize(std::string(__desc_w.begin(),__desc_w.end()));
}

void DXApplication::InitializeFrameBuffers()
{
    sceneBuffer = new DXFrameBuffer(m_d3dDevice.Get(), m_d3dContext.Get());
    outputBuffer = new DXFrameBuffer(m_d3dDevice.Get(), m_d3dContext.Get());

    FrameBufferDesc desc{};
    desc.bDepthStencil = true;
    desc.height = m_clientH;
    desc.width = m_clientW;
    desc.nRenderPass = 1;
    sceneBuffer->Create(desc);
    outputBuffer->Create(desc);

    OnResize();
}

void DXApplication::InitializeDebugLayer()
{
    ComPtr<ID3D11Debug> debugLayer;
    m_d3dDevice->QueryInterface(IID_PPV_ARGS(&debugLayer));
    debugLayer->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}

void DXApplication::CreateOutputBufferResources()
{
    VertexPT quadVertices[] = {
    { {-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f} },
    { { 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f} },
    { {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} },
    { { 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} }
    };

    DWORD quadIndices[] = {
        0, 1, 2,
        1, 3, 2
    };

    //Creating Vertex Buffer
    pOutputVertexBuffer = new DXVertexBuffer();
    VertexBufferDesc vbdesc;
    vbdesc.cbSize = sizeof(VertexPT) * 4;
    vbdesc.cbStride = sizeof(VertexPT);
    vbdesc.pData = quadVertices;
    pOutputVertexBuffer->Create(vbdesc);

    //Creating Index Buffer
    pOutputIndexBuffer = new DXIndexBuffer();
    IndexBufferDesc ibd{};
    ibd.cbSize = 6 * sizeof(DWORD);
    ibd.pData = quadIndices;
    pOutputIndexBuffer->Create(ibd);

    ShaderInformation vsinfo{};
    vsinfo.displayName = "PP_VS";
    vsinfo.shaderType = "Vertex";
    vsinfo.entryPoint = "VS";
    vsinfo.hlslFile = "Shaders\\PP_VS.hlsl";
    vsinfo.csoName = "Shaders\\PP_VS.cso";

    pOutputVertexShader = ShaderCache::CreateVertexShader(vsinfo);

    ShaderInformation psinfo{};
    psinfo.displayName = "PP_PS";
    psinfo.shaderType = "Pixel";
    psinfo.entryPoint = "PS";
    psinfo.hlslFile = "Shaders\\PP_PS.hlsl";
    psinfo.csoName = "Shaders\\PP_PS.cso";

    pOutputPixelShader = ShaderCache::CreatePixelShader(psinfo);

    pOutputVertexShader->CreateInputLayout(VertexPT::inputLayout, std::size(VertexPT::inputLayout));
}

void DXApplication::RenderOutputBuffer()
{
    pOutputVertexBuffer->BindPipeline(0);
    pOutputIndexBuffer->BindPipeline(0);
    pOutputVertexShader->Bind();
    pOutputVertexShader->UpdateInputLayout();
    pOutputPixelShader->Bind();
    ID3D11ShaderResourceView* srv = sceneBuffer->mRenderPass[0]->GetShaderResourceView();
    m_d3dContext->PSSetShaderResources(0, 1, &srv);
    m_d3dContext->PSSetSamplers(0, 1, DXRasterizerState::SSClamp.GetAddressOf());
    m_d3dContext->DrawIndexed(6, 0, 0);

    ID3D11ShaderResourceView* const nullSRV[1] = { (ID3D11ShaderResourceView*)0 };
    m_d3dContext->PSSetShaderResources(0, 1, nullSRV);
}
