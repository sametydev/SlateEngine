#include <TestEngine/Engine/Game/Game.h>
#include <TestEngine/Engine/Vertex.h>
#include <TestEngine/Engine/Buffer.h>
#include <TestEngine/Engine/Editor/EditorUI.h>

const D3D11_INPUT_ELEMENT_DESC VertexPC::inputLayout[2] = {
    { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

Game* Game::Instance = nullptr;


//TEMPORARY!!
float Game::x = 0.0f;
float Game::y = 0.0f;
float Game::py = 0.0f;
float Game::tx = 0.0f;
float Game::scale = 1.0f;


Game::Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : DXApplication(hInstance, windowName, initWidth, initHeight)
{
    if (!Instance)
    {
        Instance = this;
    }
}

Game::~Game()
{
}


bool Game::OnInit()
{
    if (!DXApplication::OnInit()) { return 0; }

    EditorUI::instance()->OnInit();

    //-- Compiling Shader
    HR(CreateShaderFromFile(L"Shaders\\TestVS.cso", L"Shaders\\TestVS.hlsl", "VS", "vs_5_0", m_blob.ReleaseAndGetAddressOf()));
    HR(m_d3dDevice->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()));
    HR(m_d3dDevice->CreateInputLayout(VertexPC::inputLayout, ARRAYSIZE(VertexPC::inputLayout),
        m_blob->GetBufferPointer(), m_blob->GetBufferSize(), m_inputLayout.GetAddressOf()));
    HR(CreateShaderFromFile(L"Shaders\\TestPS.cso", L"Shaders\\TestPS.hlsl", "PS", "ps_5_0", m_blob.ReleaseAndGetAddressOf()));
    HR(m_d3dDevice->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));
    //-----

    VertexPC vertices[] =
    {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
    };

    //Creating Vertex Buffer Descriptor
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    //Creating Subresource Data
    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertices;

    //Creating Vertex Buffer
    HR(m_d3dDevice->CreateBuffer(&bd, &sd, m_vertexBuffer.GetAddressOf()));

    unsigned long indices[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 1,
        1, 0, 4,
        1, 5, 6,
        6, 2, 1,
        7, 6, 5,
        5, 4, 7,
        3, 2, 6,
        6, 7, 3,
        4, 0, 3,
        3, 7, 4
    };

    //Index Buffer Descriptor and Creating Index Buffer
    D3D11_BUFFER_DESC ibd{};
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof indices;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    sd.pSysMem = indices;

    HR(m_d3dDevice->CreateBuffer(&ibd, &sd, m_indexBuffer.GetAddressOf()));
    m_d3dContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    //-------------------
    
    //Constant Buffer Descriptor and Creating Constant Buffer Descriptor
    D3D11_BUFFER_DESC cbd{};
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HR(m_d3dDevice->CreateBuffer(&cbd, nullptr, m_constantBuffer.GetAddressOf()));
    //-------------------

    //ConstantBuffer object for shader matrices etc.
    cbuffer.world = XMMatrixIdentity(); 
    cbuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ));
    cbuffer.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    cbuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, GetAspectRatio(), 1.0f, 1000.0f));

    //Preparing the vertex buffer things
    UINT stride = sizeof(VertexPC);
    UINT offset = 0;
    m_d3dContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

    // Set Vertex and Pixel Shaders
    m_d3dContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_d3dContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    //Binding constant buffer to vertex shader and pixel shader
    m_d3dContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
    m_d3dContext->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());


    //Setting topology for drawing;
    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Setting the input layout;
    m_d3dContext->IASetInputLayout(m_inputLayout.Get());

    return true;
}

void Game::OnResize()
{
    DXApplication::OnResize();
}

void Game::OnUpdateScene(float deltaTime)
{

    py += 0.17f * deltaTime, tx += 0.27f * deltaTime;
    py = XMScalarModAngle(py);
    tx = XMScalarModAngle(tx);

    EditorUI::instance()->OnUpdate();

    cbuffer.world = XMMatrixTranspose(
        XMMatrixScalingFromVector(XMVectorReplicate(scale)) *
        XMMatrixRotationX(py) * XMMatrixRotationY(tx) *
        XMMatrixTranslation(x, y, 0.0f));
    cbuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(1.37f, GetAspectRatio(), 1.0f, 1000.0f));



    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(m_d3dContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(cbuffer), &cbuffer, sizeof(cbuffer));
    m_d3dContext->Unmap(m_constantBuffer.Get(), 0);

}

void Game::OnRenderScene()
{
    static float rgba[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), rgba);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->DrawIndexed(36, 0, 0);
    EditorUI::instance()->OnRender();


    HR(m_swapChain->Present(0, 0));
}