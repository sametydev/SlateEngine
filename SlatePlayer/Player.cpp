#include "Player.h"


Player* Player::Instance = nullptr;

Player::Player()
{
	game = Game::Instance;
	if (!Instance)
	{
		Instance = this;
	}
}

Player::~Player()
{
}

void Player::OnInit(HWND wnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
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
    dxVertexBuffer = new DXVertexBuffer();
    VertexBufferDesc vbdesc;
    vbdesc.cbSize = sizeof(VertexPT) * 4;
    vbdesc.cbStride = sizeof(VertexPT);
    vbdesc.pData = quadVertices;
    dxVertexBuffer->Create(vbdesc);

    //Creating Index Buffer
    dxIndexBuffer = new DXIndexBuffer();
    IndexBufferDesc ibd{};
    ibd.cbSize = 6 * sizeof(DWORD);
    ibd.pData = quadIndices;
    dxIndexBuffer->Create(ibd);

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    pDevice->CreateSamplerState(&samplerDesc, &samplerState);

    ShaderInformation vsinfo{};
    vsinfo.displayName = "FullscreenQuadVS";
    vsinfo.shaderType = "Vertex";
    vsinfo.entryPoint = "VS";
    vsinfo.hlslFile = "Shaders\\FullscreenQuadVS.hlsl";
    vsinfo.csoName = "Shaders\\FullscreenQuadVS.cso";

    vertexShader = ShaderCache::CreateVertexShader(vsinfo);

    ShaderInformation psinfo{};
    psinfo.displayName = "FullscreenQuadPS";
    psinfo.shaderType = "Pixel";
    psinfo.entryPoint = "PS";
    psinfo.hlslFile = "Shaders\\FullscreenQuadPS.hlsl";
    psinfo.csoName = "Shaders\\FullscreenQuadPS.cso";

    pixelShader = ShaderCache::CreatePixelShader(psinfo);

    vertexShader->CreateInputLayout(VertexPT::inputLayout, std::size(VertexPT::inputLayout));
}

void Player::NewFrame()
{

}

void Player::OnRender(float rgba[4])
{
    
}

float rgba[4] = { 0,0,0,0 };

void Player::OnRenderScene(ID3D11DeviceContext* pContext)
{
    game->ClearRenderTarget(rgba);

    dxVertexBuffer->BindPipeline(0);
    dxIndexBuffer->BindPipeline(0);
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    vertexShader->Bind();
    vertexShader->UpdateInputLayout();
    pixelShader->Bind();

    ID3D11ShaderResourceView* srv = Game::Instance->GetRenderTarget()->mRenderPass[0]->GetShaderResourceView();
    pContext->PSSetShaderResources(0, 1, &srv);
    pContext->PSSetSamplers(0, 1, &samplerState);

    pContext->DrawIndexed(6, 0, 0);

    ID3D11ShaderResourceView* const nullSRV[1] = { (ID3D11ShaderResourceView*)0 };
    pContext->PSSetShaderResources(0, 1, nullSRV);
}

void Player::OnUpdate(float deltaTime)
{
}

void Player::ClearViewport(float rgba[4])
{
}

void Player::ResizeViewport(int w, int h)
{
}

void Player::HandleInput(float deltaTime)
{
}

LRESULT Player::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
