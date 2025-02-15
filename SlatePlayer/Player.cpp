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
    context = pDeviceContext;
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
    game->ClearRenderTarget(rgba);

    dxVertexBuffer->BindPipeline(0);
    dxIndexBuffer->BindPipeline(0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    vertexShader->Bind();
    vertexShader->UpdateInputLayout();
    pixelShader->Bind();

    ID3D11ShaderResourceView* srv = Game::Instance->GetOutputBuffer()->mRenderPass[0]->GetShaderResourceView();
    context->PSSetShaderResources(0, 1, &srv);
    context->PSSetSamplers(0, 1, DXRasterizerState::SSClamp.GetAddressOf());

    context->DrawIndexed(6, 0, 0);

    ID3D11ShaderResourceView* const nullSRV[1] = { (ID3D11ShaderResourceView*)0 };
    context->PSSetShaderResources(0, 1, nullSRV);
}

float rgba[4] = { 0,0,0,0 };

void Player::OnRenderScene(ID3D11DeviceContext* pContext)
{
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
